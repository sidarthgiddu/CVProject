import processing.video.*;
import monclubelec.javacvPro.*;
import processing.net.*;


int prevVelocity = 1;
int prevTurn = 2003;

int MIN_SPEED_FRONT = 0;
int MIN_SPEED_BACK = 0;
int MIN_TURN_LEFT = 550;  // More radius; means less turn
int MIN_TURN_RIGHT = -550;  // more radius: means less turning

int SPEED_INCREMENT_STEPS = 4;
int TURN_INCREMENT_STEPS = 20;

int MAX_SPEED_FRONT = 500;
int MAX_SPEED_BACK = -500;
int MAX_TURN_LEFT = 201;  // with 0 radius, turning will be maximum
int MAX_TURN_RIGHT = -201;  // with 0 radius, turning will be maximum

int velocity = MIN_SPEED_FRONT;
int turn = MIN_TURN_LEFT;

boolean isUpPressed = false;
boolean isDownPressed = false;
boolean isLeftPressed = false;
boolean isRightPressed = false;

//boolean isCtrlPressed = false;
//boolean isRPressed = false;
byte serialDataOverInternet[] = { 
  0, 0, 0, 0
};

// Communication with Arduino
Client client;
String arduinoIpAddress = "192.168.137.10";
//String arduinoIpAddress = "127.0.0.1";

void setup() {
  
   client = new Client(this, arduinoIpAddress, 23);
//  client = new Client( this, arduinoIpAddress, 6881);
}


void draw() {
//  background(0);
//  fill(255);
//  if (isCtrlPressed) background(255, 0, 0);
//  if (isRPressed) background(0, 255, 0);
//  if (isCtrlPressed && isRPressed) background(255, 255, 0);

  // Front speed velocity range: [0, 500]
  if ( isUpPressed && velocity < MAX_SPEED_FRONT) velocity = velocity + SPEED_INCREMENT_STEPS;
  
  // Rear speed velocity range: [0, -500]
  if ( isDownPressed && velocity > MAX_SPEED_BACK) velocity = velocity - SPEED_INCREMENT_STEPS;
  
  // Left turn radius range [2000, 1]
  if ( isLeftPressed && turn > MAX_TURN_LEFT) turn = turn - TURN_INCREMENT_STEPS;
  
  // Right turn radius range [-2000, -1]
  if ( isRightPressed && turn < MAX_TURN_RIGHT) turn = turn + TURN_INCREMENT_STEPS;
  
  if((prevVelocity != velocity) || (prevTurn != turn)) {  
    iRobotCommandFormatVelocity();
  
    iRobotCommandSendingToArduino();
  }
  // iRobotCommandFormatRadius();
  
  println( "velocity = " + binary(velocity) + " turn = " + binary(turn));
  println( "Velocity = " + velocity + " Turn = " + turn);
  
  // Club it into the package and send it to Arduino.
  println( "serialData[0,1,2,3] = " + (int)serialDataOverInternet[0] + " " + (int)serialDataOverInternet[1] + " " + 
          (int)serialDataOverInternet[2] + " " + (int)serialDataOverInternet[3]);
}

void iRobotCommandFormatVelocity() {
  
  // [Velocity_HigherOrder][Velocity_LowerOrder][Radius_HighOrder][Radius_LowerOrder]
  serialDataOverInternet[0] = (byte)((velocity & 0xFF00) >> 8);
  serialDataOverInternet[1] = (byte)(velocity & 0x00FF);
  
  serialDataOverInternet[2] = (byte)((turn & 0xFF00) >> 8);
  serialDataOverInternet[3] = (byte)(turn & 0x00FF);
}

// Send data byte by byte
void iRobotCommandSendingToArduino() {
  client.write(serialDataOverInternet[0]);
  client.write(serialDataOverInternet[1]);
  client.write(serialDataOverInternet[2]);
  client.write(serialDataOverInternet[3]);
  prevVelocity = velocity;
  prevTurn = turn;
  // delay(500);
}

void keyPressed() {
//  if (keyCode == CONTROL && isCtrlPressed == false) isCtrlPressed = true;
//  if (char(keyCode) == 'R') isRPressed = true;

  // Only allow up and down to be pressed once. In draw, we will increment the speed if pressed is true.
  if (keyCode == UP && isUpPressed == false) {
    isUpPressed = true;
    // stop backward movement if up button is pressed.
    //isDownPressed = false;
    velocity = MIN_SPEED_FRONT;
  }
  if (keyCode == DOWN && isDownPressed == false) {
    isDownPressed = true;
    // Stop forward movement if down button is pressed
    //isUpPressed = false;
    velocity = MIN_SPEED_BACK;    
  }
  if (keyCode == LEFT && isLeftPressed == false) {
    isLeftPressed = true;
    // stop right movement if left button is pressed
    //isRightPressed = false;
    turn = MIN_TURN_LEFT;    
  }

  if (keyCode == RIGHT && isRightPressed == false) {
    isRightPressed = true;
    // stop left movement is right button is pressed
    //isLeftPressed = false;
    turn = MIN_TURN_RIGHT;
  }
}

void keyReleased() {
//  if (keyCode == CONTROL) isCtrlPressed = false;
//  if (char(keyCode) == 'R') isRPressed = false;

  // Whenever we release up arrow, reset the speed to min
  if (keyCode == UP) {
    isUpPressed = false;
    velocity = MIN_SPEED_FRONT;
  }
  
  // Whenever we release down arrow, reset the speed to min
  if (keyCode == DOWN) {
    isDownPressed = false;
    velocity = MIN_SPEED_BACK;
  }
  
  // Whenever we release left arrow and we have not already pressed right arrow, reset the radius to min
  if (keyCode == LEFT) {
    isLeftPressed = false;
    if ( !isRightPressed ) {
      turn = MIN_TURN_LEFT;
    }
  }

  // whenever we release right arrow and we have not already pressed left arrow, reset the radius to min
  if (keyCode == RIGHT) {
    isRightPressed = false;
    if ( !isLeftPressed ) {
     turn = MIN_TURN_RIGHT;
    }
  }
}



/*
void draw() {
 background(0);
 fill(255);
 if (isCtrlPressed) background(255, 0, 0);
 if (isRPressed) background(0, 255, 0);
 if (isCtrlPressed && isRPressed) background(255, 255, 0);
 
 if ( isUpPressed && speedFront < MAX_SPEED_FRONT) speedFront = speedFront + 2;
 else if ( !isUpPressed ) speedFront = MIN_SPEED_FRONT;
 
 // backwards movement. 
 if ( isDownPressed && speedBack > MAX_SPEED_BACK) speedBack = speedBack - 2;
 else if ( !isDownPressed ) speedBack = MIN_SPEED_BACK;
 
 if ( isLeftPressed && turnLeft > MIN_TURN_LEFT) turnLeft = turnLeft - 10;
 else if ( !isLeftPressed ) turnLeft = MIN_TURN_LEFT;
 
 if ( isRightPressed && turnRight < MAX_TURN_RIGHT) turnRight = turnRight + 10;
 else if ( !isRightPressed ) turnRight = MIN_TURN_RIGHT;
 
 iRobotCommandFormatVelocity();
 
 // iRobotCommandFormatRadius();
 
 println( "UpSpeed = " + speedFront + " DownSpeed = " + speedBack + " LeftSpeed = " + turnLeft + " RightSpeed = " + turnRight);
 println( "serialData = " + (int)serialDataOverInternet[0]);
 }
 
 void iRobotCommandFormatVelocity() {
 
 }
 
 void keyPressed() {
 if (keyCode == CONTROL && isCtrlPressed == false) isCtrlPressed = true;
 if (char(keyCode) == 'R') isRPressed = true;
 
 // Only allow up and down to be pressed once. In draw, we will increment the speed if pressed is true.
 if (keyCode == UP && isUpPressed == false) {
 isUpPressed = true;
 // stop backward movement if up button is pressed.
 isDownPressed = false;
 }
 if (keyCode == DOWN && isDownPressed == false) {
 isDownPressed = true;
 // Stop forward movement if down button is pressed
 isUpPressed = false;
 }
 if (keyCode == LEFT && isLeftPressed == false) {
 isLeftPressed = true;
 // stop right movement if left button is pressed
 isRightPressed = false;
 }
 
 if (keyCode == RIGHT && isRightPressed == false) {
 isRightPressed = true;
 // stop left movement is right button is pressed
 isLeftPressed = false;
 }
 }
 void keyReleased() {
 if (keyCode == CONTROL) isCtrlPressed = false;
 if (char(keyCode) == 'R') isRPressed = false;
 
 if (keyCode == UP) isUpPressed = false;
 if (keyCode == DOWN) isDownPressed = false;
 if (keyCode == LEFT) isLeftPressed = false;
 if (keyCode == RIGHT) isRightPressed = false;
 }
 */


/*
void keyPressed(KeyEvent keyEvent) {
 
 if (key == CODED) {
 if (keyCode == UP && speedFront < MAX_SPEED_FRONT) {
 speedFront++;
 println("UP" + speedFront);
 }
 
 if (keyCode == LEFT && turnLeft < MIN_TURN_LEFT) {
 turnLeft++;
 println("Left" + turnLeft);
 }
 
 if (keyCode == RIGHT && turnRight < MAX_TURN_RIGHT) {
 turnRight++;
 println("Right" + turnRight);
 }
 
 if (keyCode == DOWN && speedBack < MAX_SPEED_BACK)
 {
 
 speedBack++;
 println("Down " + speedBack);
 }
 }
 }
 
 
 void keyReleased(KeyEvent ke) {
 println(ke.getKeyCode());;
 println("Released");
 speedFront = 0;
 speedBack = 0; 
 turnLeft = 0;
 turnRight = 0;
 }
 */
