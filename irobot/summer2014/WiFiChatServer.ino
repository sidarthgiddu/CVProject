/*
 Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 
 Circuit:
 * WiFi shield attached
 
 created 18 Dec 2009
 by David A. Mellis
 modified 31 May 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "irobot"; //  your network SSID (name) 
char pass[] = "irobotirobot";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
int dir;
int vel;

WiFiServer server(23);

//int led = 13;

boolean alreadyConnected = false; // whether or not the client was connected previously

void MOVE(byte speedHigh, byte speedLow, byte radiusHigh, byte radiusLow) {
  Serial1.write(137); // Opcode number for DRIVE
  // Velocity 
  Serial1.write(speedHigh); 
  Serial1.write(speedLow);
  // Radius 
  Serial1.write(radiusHigh); 
  Serial1.write(radiusLow); 
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(57600); 
  Serial1.begin(57600);
  Serial1.write(128); // It starts OI
  Serial1.write(132); // set to be full mode
  //pinMode(led, OUTPUT);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
      // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 

  // start the server:
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
 }


void loop() {
  // wait for a new client:
  WiFiClient client = server.available();


  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      //client.flush();    
      // Serial.println("We have a new client");
      //client.println("Hello, client!"); 
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      byte thisChar = client.read();
      vel = (int)(thisChar)%10;
      dir = ((int)(thisChar)-vel)/10;
      vel = vel*50;
      server.write(thisChar);
      //Serial.println(thisChar);
      //Serial.println(dir);
      
      // North
      if(dir == 4){
        MOVE(0, vel, 128, 0);
      }
      // East
      if(dir == 5){
        vel = vel - 50;
        MOVE(255, (256-vel), 0, 0);
        //delay(1000);
      }
      // South
      if(dir == 6){ 
        
        MOVE(255, (256-vel), 128, 0);
      }
      // West
      if(dir == 7){
        vel = vel - 50;
        MOVE(0, vel, 0, 0);
        //delay(1000);
      }            
      // Stop
      if(dir == 8){
        MOVE(0, 0, 0, 0);
      }
    }
  }
}



void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


