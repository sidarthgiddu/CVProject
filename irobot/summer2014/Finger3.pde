import processing.video.*;
import monclubelec.javacvPro.*;
import processing.net.*;

Capture video;
int wscreen=640;
int hscreen=480;
color[] vline;

PImage img;
PImage img2;
PImage im;
float now = millis();
float timing = millis();
float turning = millis();
boolean turn = false;
float dx;
float dy;
byte speed;
float angle;
boolean run = false;
boolean st = false;
byte save = 85;
byte save2 = 85;
int counter = 0;

Client client;
String id= "192.168.137.10";

float posx = 0;
float posy = 480;
float prex = 480;
float prey = 360;

OpenCV opencv; 

Blob[] blobsArray=null; // tableau pour la détection des blobs (contour de forme)

ConvexityDefect[] cdArray=null;

void setup() {
    size(640, 480);
    vline = new color[wscreen];  
    // The videoera can be initialized directly using an 
    // element from the array returned by list():
    video = new Capture(this);
    client = new Client(this, id, 23);
    video.start();     
    opencv = new OpenCV(this); // initialise objet OpenCV à partir du parent This
    opencv.allocate(320, 240);    
}

void draw() {
  
// mirror the video
if (video.available() == true) {
    video.read();
    video.loadPixels();
    for(int b=0;b<hscreen;b++) {
        int ig=b*wscreen;
        for(int a=0; a<wscreen; a++) 
          vline[a] = video.pixels[ig+a];
        for(int a=0; a<wscreen; a++) 
          video.pixels[ig+a]=vline[wscreen-1-a];
      }
      img = video.get(0,0,320,240);
      while((millis()-now) < 6000){
      img2 = img;
    }
    image(img,0,0,320,240);
    image(img2,320,0,320,240);
    
    opencv.copy(img2); //  Load in the buffer pImage OpenCV 
    opencv.remember(); // Keep the buffer in the buffer memory 
    image(opencv.getMemory(),0,0); // Output buffer memory OpenCV in Processing window 
    
    opencv.copy(img); //  Load in the buffer pImage OpenCV 
    image(opencv.getBuffer(),opencv.width(),0); //  Display the main buffer OpenCV in Processing window 


        //---  Function Application absDiff 
        opencv.absDiff(); // Do subtraction and principal Memory Buffer - Memory in result set 2 

        image(opencv.getMemory2(),0,opencv.height()); // 3rd
        //--- pplying a binary thresholding to isolate detected object ---
        
        opencv.threshold(opencv.Memory2,0.15, "BINARY"); //  Binary thresholding to remove background - can lower threshold eliminated as background 
        image(opencv.getMemory2(),opencv.width(),opencv.height()); // Output buffer Memory2 OpenCV in Processing window


        //--- Detection Blobs --- 
        blobsArray=opencv.blobs(opencv.Memory2, opencv.area()/100,opencv.area(),20, true, 1000, true); 
        // Apply blobs to IplImage object with parameters and returns array of objects Blob

        //-- Drawing rectangle around the path of the form -- 
         //opencv.drawRectBlobs(blobsArray,opencv.width(),opencv.height(),1); 
         //  Trace rectangle based on reference points and the parameters 

        //-- Drawing the perimeter of the blob on the original image --
          opencv.drawBlobs(blobsArray,opencv.width(),opencv.height(),1 ); 
          // Record forms Blobs table based on reference point + default 

        //-- Drawing the center of the blob on the layout of the form -- 
          //opencv.drawCentroidBlobs (blobsArray,opencv.width(),opencv.height(),1); 
          // Trace the center of Blob based on reference point + scale and with the default settings
        
       for (int i=0; i<blobsArray.length; i++) { // reviews the blobs
         
          // ConvexityDefect[] convexityDefects(Blob blobIn, float minDepthIn, float angleMaxIn, boolean debugIn)
          cdArray=opencv.convexityDefects(blobsArray[i], (float)20, radians(110), true); 

          //drawConvexityDefects(ConvexityDefect[] cdArrayIn, int xRefIn, int yRefIn, float scaleIn, int radius, int colorStrokeIn, int strokeWeightIn, boolean fillIn, int colorFillIn, boolean lineIn, int colorStrokeLineIn, int strokeWeightLineIn, int mode, boolean debugIn )
          opencv.drawConvexityDefects(cdArray, opencv.width(),opencv.height(),1.0, 10,color(0,255,255), 2, false, 0, true, color(0,0,255), 2, 1, false ); 


        }// fin for i - défile blobs  
        
        // Find highest point
        im = opencv.getMemory2();
        im.loadPixels();
        
        // Get points only from blobs
        for(int k=0; k<blobsArray.length; k++){
            counter = counter + 1;
            for(int p=0; p<blobsArray[k].points.length; p++){
              if(blobsArray[k].points[p].y < posy){
                posy = blobsArray[k].points[p].y;
                posx = blobsArray[k].points[p].x;
              }               
          }
        }        
               
        // Timing and calculate speed
        // Action based movement
        // 1482 ms = 30 degress turned
        if(turn == false || (turn == true && millis()-turning > 1482)){
          turn = false;
          turning = millis();
        if(run == true && millis()-timing > 10){
          timing = millis(); // restart timing
          dx = posx - prex; // x difference
          println("X: "+posx);
          println("Y: "+posy);
          dy = prey - posy; // y difference
          speed = byte(sqrt(dy*dy+dx*dx)); // used to determine speed
          speed = byte((speed-12)/20);
          // Change to speed level(0-4)
          if(speed > 4){
            speed = 4;
          }                    
          
          if(dx == 0){
            angle = 0;
          }
          else{
            angle = atan(dy/dx);
            if(dx < 0){
              angle = angle + PI;
            }
          }
          
          if(counter == 0){
            println("Stop");
            client.write(85);
            
          }
          else if(speed <= 1){
            if(save == 85){
              println("Stop");
              client.write(85);
            }
            else{              
              println(save);
              client.write(save);
              
            }
            
            // Continue UP or DOWN movement when no movement is detected
            client.write(save);
          }
          else{
          if(prey > 90 && posy < 90){ //Up
            //client.write(char(40+speed));
            //client.write(speed);
            println("UP  "+(40+speed));
            save = byte(speed+40);
            client.write(save);
            turn = false;
            
          }
          if(prex < 200 && posx > 200){ //Right
            //client.write(char(50+speed));
            //client.write(speed);
            println("Right  "+(50+speed));
            client.write(byte(50+speed));
            save2 = byte(speed+50);
            save = 85;
            turning = millis();
            turn = true;
            
          }
          if(prex > 120 && posx < 120){ //Left
            //client.write(char(70+speed));
            //client.write(speed);
            println("Left  "+(70+speed));
            client.write(byte(70+speed));
            save2 = byte(speed+70);
            save = 85;
            turning = millis();
            turn = true;
            
          }
          if(prey < 150 && posy > 150){ //Down
            //client.write(char(60+speed));
            //client.write(speed);
            println("Down  "+(60+speed));
            save = byte(speed+60);
            client.write(save);
            turn = false;
            
          }
          }
        }
        }
        else{
          //client.write(save2);
          st = false;
        }
        if(run == false){
          prex = posx;
          prey = posy;
          run = true;
        }

        
        
        // Position based movement
        if(posx<200 && posx > 120 && posy>80 && posy<160){
          //client.write(85);
        }
        else{
        if(posx > 160 && abs(posy-120)<(posx-160)){// Right
          //client.write(char(51));
          //println("Right");
          //println("("+posx+","+posy+")");
        }
        if(posx <= 160 && abs(posy-120)<(160-posx) && posx != 0){// Left
          //client.write(char(71));
          //println("Left");
          //println("("+posx+","+posy+")");
        }
        if(posy <= 120 && posy != 0 && (120-posy)>abs(160-posx)){// Up
          //client.write(char(41));
          //println("Up");
          //println("("+posx+","+posy+")");
        }
        if(posy >=120 && (posy-120)>abs(160-posx)){// Down
          //client.write(char(61));
          //println("Down");
          //println("("+posx+","+posy+")");
        }
        if(posy == im.height && posx == 0){
          //client.write(char(85));
          //println("Stop");
        }
        }
        
        // save current position to previous position
          prex = posx;
          prey = posy;
          
        //line(320,0,320,240);
        line(120,0,120,240);
        line(200,0,200,240);
        line(440,0,440,240);
        line(520,0,520,240);
        line(0,90,640,90);
        line(0,150,640,150);
        rectMode(CENTER);
        rect(prex+320,prey,40,40);
        //rect(prex+320,prey,40,40);
        //rect(480,120,80,80);
        posx = 0;
        posy = im.height;
        counter = 0;
}

 
}

void keyPressed() {
  
  if (key == CODED) {
    if (keyCode == UP) {
      client.write(43);
      println("UP");
    } 
    else if (keyCode == DOWN) {
      client.write(62);
      println("DOWN");
    } 
    else if (keyCode == LEFT){
      client.write(72);
      println("LEFT");
    }
    else if (keyCode == RIGHT){
      client.write(52);
      println("RIGHT");
    }
  }
  
}

void keyReleased(){
  client.write(85);
  println("STOP");
}
