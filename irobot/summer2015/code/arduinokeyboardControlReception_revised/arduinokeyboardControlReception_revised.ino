// Import the SPI library (Serial Peripheral Interface, to enable communication of Galileo with peripheral devices)
#include <SPI.h>
// Import the WiFi library. 
#include <WiFi.h>

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
int counter; // indicates how many bytes are currently loaded in the buffer

// Flags
boolean stopCommandFlag = false;  // Prevents data to be sent multiple times to iRobot
boolean alreadyConnected = false; // whether or not the client was connected previously

//OpCodes for iRobot: 
int start_OI = 128;
int set_FullMode = 132;
int drive_iRobot = 137; // must be in Full Mode to drive iRobot


// 1 time seting up. One the press of reset.
void setup() {
  // Initialize serial and wait for port to open:
  // Serial refers to USB Client port. It is responsible for sending programs from other devices to Intel Galileo board. 
  // Serial1 refers to Pin 0 and Pin 1 in Intel Galileo digital pins
  // We use Serial1 to talk to iRobot, and Serial to enable communication between Client and Intel Galileo. 
  
  // .begin() sets the data rate in bits per second (baud) for serial data transmission. 
  Serial.begin(57600); 
  Serial1.begin(57600);
  
  Serial1.write(start_OI);
  Serial1.write(set_FullMode); 
  
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
      
      // counter records the number of bytes loaded in the data buffer. Clear it before receiving new data. 
      if( counter == 4 ) {
        counter = 0;
      
        // If the stop commmand is sent from FPGA, reset the values to stop iRobot
        if ( receivedData[0] == 255 &&
             receivedData[1] == 255 &&
             receivedData[2] == 255 &&
             receivedData[3] == 255)
        {
             if (!stopCommandFlag) {
                Serial1.write(start_OI); // It starts OI
                Serial1.write(set_FullMode); // set to be full mode
             }
             
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
        iRobotDriveCommands();
      }
      
      // Override the 4 bytes iRobot has just executed with new data, one byte in each loop. 
      // Caviate: We can't receive 0 from the network, because it's NULL ASCII character and NULLs can't be transmitted over network.
      receivedData[counter++] = client.read();
    }
    
    else if (counter >= 4) {
     receivedData[0]=0;
     receivedData[1]=0;
     receivedData[2]=0;
     receivedData[3]=0;
    }
  }    
}



// Parse and send each byte to Serial1 port (to iRobot) as well as feedback through Serial
void iRobotDriveCommands() {
  Serial1.write(drive_iRobot);
  Serial1.write(receivedData[0]);
  Serial.print(" Byte1 = "); Serial.print((int)receivedData[0]);
  Serial1.write(receivedData[1]);
  Serial.print(" Byte2 = "); Serial.print((int)receivedData[1]);
  Serial1.write(receivedData[2]);
  Serial.print(" Byte3 = "); Serial.print((int)receivedData[2]);
  Serial1.write(receivedData[3]);
  Serial.print(" Byte4 = "); Serial.println((int)receivedData[3]);
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
