#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
// Import the SPI library (Serial Peripheral Interface, to enable communication of Galileo with peripheral devices)
#include <SPI.h>
// Import the WiFi library. 
#include <WiFi.h>

#define MAX_NO_LEFT 550
#define MIN_NO_LEFT 500
#define MAX_NO_RIGHT -550
#define MIN_NO_RIGHT -500
#define MAX_PWM 255
#define MIN_PWM 0
#define MAX_SPEED 500
#define MIN_SPEED 0
#define ROBOT_HALF_WIDTH 73.0 // in milimeters

// Declare the WiFi that Galileo should receive data from. 
char ssid[] = "eyerobot"; //  your network SSID (name) 
char pass[] = "eyerobot";    // your network password (use for WPA, or use as key for WEP)

// Parameter for the current status of Galileo's WiFi radio 
int status = WL_IDLE_STATUS;

// Server is Galileo, and Client is whoever sends out the data (Processing, Visual Studio, or FPGA)
// Server is hosted for port 23.
WiFiServer server(23);

// Can only send one byte of data via Serial port a time, so Galileo needs a buffer to collect a data packet of 4 bytes 
// (2 for velocity and 2 for orientation) sent from the client (either FPGA or Visual Studio) before sending to iRobot
byte receivedData[4];
int counter = 0; // indicates how many bytes are currently loaded in the buffer

// Flags
boolean stopCommandFlag = false;  // Prevents data to be sent multiple times to iRobot
boolean alreadyConnected = false; // whether or not the client was connected previously

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *Motor_Left_Front = AFMS.getMotor(4);   // Motor 4 
Adafruit_DCMotor *Motor_Right_Front = AFMS.getMotor(3);;  // Motor 3
Adafruit_DCMotor *Motor_Left_Rear = AFMS.getMotor(1);   // Motor 1
Adafruit_DCMotor *Motor_Right_Rear = AFMS.getMotor(2);   // Motor 2

short speed;
short radius;
short pwm;
float pwmLeft_before;
float pwmRight_before;
byte pwmLeft;
byte pwmRight;


// 1 time setting up. On the press of reset.
void setup() {
  // Initialize serial and wait for port to open:
  // Serial refers to USB Client port. It is responsible for sending programs from other devices to Intel Galileo board. 
  // We use Serial1 to talk to iRobot, and Serial to enable communication between Client and Intel Galileo. 
  
  // .begin() sets the data rate in bits per second (baud) for serial data transmission. 
  Serial.begin(9600);  
  
  // check for the presence of the shield. For Galileo, we use a mPCIe WiFi card instead of WiFi sheild. 
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
      // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network. 
    // Returns the status of the WiFi radio: WL_CONNECTED or WL_IDLE_STATUS (not connected to a network but powered on)
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 

  // start the server:
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
   // start the motor sheild
  AFMS.begin(); 
 }



// After connecting to WiFi in setup(), start data transfer in loop()
void loop() {
  
  // wait for a new client:
  WiFiClient client = server.available();
  
  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      //client.flush();    
      Serial.println("We have a new client");
      //client.println("Hello, client!"); 
      alreadyConnected = true;
    } 
    
    // Fetch incoming data if the server is available to receive and the client is available to send. 
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      // Override the 4 bytes iRobot has just executed with new data, one byte in each loop. 
      // Caviate: We can't receive 0 from the network, because it's NULL ASCII character and NULLs can't be transmitted over network.
      
      receivedData[counter++] = client.read();
      
      // counter records the number of bytes loaded in the data buffer. Clear it before receiving new data. 
      if( counter == 4 ) {
        
        counter = 0;

        Serial.print(" Byte1 = "); Serial.print((int)receivedData[0]);
        Serial.print(" Byte2 = "); Serial.print((int)receivedData[1]);
        Serial.print(" Byte3 = "); Serial.print((int)receivedData[2]);
        Serial.print(" Byte4 = "); Serial.println((int)receivedData[3]);
      
        // If the stop commmand is sent from FPGA, reset the values to stop iRobot
        if ( receivedData[0] == 255 &&
             receivedData[1] == 255 &&
             receivedData[2] == 255 &&
             receivedData[3] == 255)
        {
             receivedData[0] = 0;
             receivedData[1] = 0;
             receivedData[2] = 0;
             receivedData[3] = 0;
             
             // Only for the first time, it will send 0. Other than that, it will set the flag to prevent it from sending
             stopCommandFlag = true;
        }
        
        // Else, Galileo should continue receiving data from Client.
        else
        {
           // If received command is reasonable, then reset the flag to false, so that 
           // it can send stop command after object is removed from the screen
           stopCommandFlag = false; 
        }
        
        // The condition of the outer loop is “counter==4”, so we have all 4 bytes of data collected in our buffer (which might have been set to zeros in the lines above). Now demand iRobot to execute these 4 bytes
        robotDriveCommands();
      }
    }
    
    else if (counter == 0) {
     receivedData[0]=0;
     receivedData[1]=0;
     receivedData[2]=0;
     receivedData[3]=0;
     robotDriveCommands();
    }
  }
}

void robotDriveCommands() {
  // Combine first two bytes received to calculate the speed (in two's complements)
  speed = (receivedData[0] <<8) + receivedData[1];
  // since motors can only have non-negative pwm, use absolute value of speed to calculate the maginitude of
  // pwm and use the sign of speed to decide FORWARD / BACKWARD /RELEASE
  pwm = map(abs(speed), MIN_SPEED, MAX_SPEED, MIN_PWM, MAX_PWM);
  
  // Combine first two bytes received to calculate the speed (in two's complements)
  radius = (receivedData[2] <<8) + receivedData[3]; 
  // Calculate the pwm for left wheels and right wheels according to the mathematic principle of similarity
  // Type casted to byte, since the argument for method setSpeed (called in function driveMotors) has to be one byte 
  
  if ((radius <= MAX_NO_LEFT & radius >MIN_NO_LEFT)||(radius >= MAX_NO_RIGHT & radius < MIN_NO_RIGHT)){ //robot moves forward and backward with no turning
      pwmLeft = (byte)pwm;
      pwmRight = (byte)pwm;
  }
  else{
    pwmLeft_before=(1 - 2*ROBOT_HALF_WIDTH / radius) * pwm;
    
    if (pwmLeft_before>MAX_PWM)
    {
      pwmLeft = MAX_PWM;
    }
    else
    {
      pwmLeft = (byte)(pwmLeft_before);
    }
    
    pwmRight_before=(1 + 2*ROBOT_HALF_WIDTH / radius) * pwm;
    
    if (pwmRight_before>MAX_PWM)
    {
      pwmRight = MAX_PWM;
    }
    else
    {
      pwmRight = (byte)(pwmRight_before);
    }
  }
  
  Serial.print("speed = ");Serial.print(speed);
  Serial.print("\t pwm = "); Serial.print(pwm);
  Serial.print("  radius = "); Serial.print(radius);
  Serial.print("  pwmLeft = "); Serial.print(pwmLeft);
  Serial.print("  pwmRight = "); Serial.print(pwmRight);
  Serial.println("");
  driveMotors();
}

void driveMotors(){
  Motor_Left_Front->setSpeed(pwmLeft);
  Motor_Left_Rear->setSpeed(pwmLeft);
  Motor_Right_Front->setSpeed(pwmRight);
  Motor_Right_Rear->setSpeed(pwmRight);
  
  if (speed<0){
    Motor_Left_Front->run(BACKWARD);
    Motor_Left_Rear->run(BACKWARD);
    Motor_Right_Front->run(BACKWARD);
    Motor_Right_Rear->run(BACKWARD);
  }
  else if (speed>0){
    Motor_Left_Front->run(FORWARD);
    Motor_Left_Rear->run(FORWARD);
    Motor_Right_Front->run(FORWARD);
    Motor_Right_Rear->run(FORWARD);
  }
  else{
    Motor_Left_Front->run(RELEASE); //stop the motor
    Motor_Left_Rear->run(RELEASE);
    Motor_Right_Front->run(RELEASE);
    Motor_Right_Rear->run(RELEASE);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield/card's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
