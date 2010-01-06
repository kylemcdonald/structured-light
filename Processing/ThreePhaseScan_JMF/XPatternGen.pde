/**
 * This file is where the video-capture and the
 * projection of the striped image happens.
 * Changes:
 * 2009-06-05 added comments and attempt to move from
 *   processing (Quicktime) to libcv (pure Java Media Framework)
 */

// Quicktie
//import processing.video.*;

// Java Media Framework
import toxi.video.capture.*;

// use the JMFStudio to find out the name of the device
String CAPTURE_DEVICE="vfw:Microsoft WDM Image Capture (Win32):0";
int CAPTURE_WIDTH = 640;
int CAPTURE_HEIGHT = 480;

SimpleCapture Img; 
secondApplet s;

float inc;
/**
 * state of our state-maching.
 * incremented every time the secondApplet:draw() method is called
 * (counter  )=1     = capture texture -image
 * (counter+1)=2-8   = draw the 3 striped images to be projected onto the object
 * (counter+1)=6,7,8 = capture frame 1, 2 and 3
 * (counter+1)=9     = save captured images to disk, reset counter, mode=false
 */
int counter;


PImage pp1;
PImage pp2;
PImage pp3;
PImage pp4;

/**
 * true = do the scan.
 * @see #counter
 */
boolean mode = false;
boolean reset = false;
float numStripes=32;
String folderPath = "/data" ;

int speed = 60;

public class PFrame extends Frame {
    public PFrame() {
        setBounds(-1100,screen.height-790,1124,790);
        background (255);
        s = new secondApplet();
        add(s);
        s.init();
        show();
        
    }
}

/**
 * SecondApplet is shown in fullscreen on a projector
 * and displays 3 images. Inside the draw()-method is
 * a state-machine that does these projections and
 * also captures a photo of the object with these
 * 3 images being projected onto it.<br/>
 * A 4th image tex.png is captured with a white surface
 * being projected onto the object to capture it's
 * texture.<br/>
 * Start this process by setting the global cariables
 * count=0 and mode=true.<br/>
 * After this process the 4 images are saved to disk and
 * the global function decodeData() is called.
 */
public class secondApplet extends PApplet {
 public void setup() {
 
   // +22 to hide the window-border
   size(screen.width, screen.height+22);
   // white background
   background (255);

 
   // Quicktime
   //Img = new Capture(this,inputWidth, inputHeight, 15);

   // Java Media Framework
   Img = new JMFSimpleCapture();
   if (!Img.initVideo(CAPTURE_DEVICE, CAPTURE_WIDTH, CAPTURE_HEIGHT, 0)) {
      println("Cannot initialize video-capture:" + Img.getError());
      System.exit(0);
   }

   // the 3 frames + combined texture we are going to capture
   pp1 = createImage (CAPTURE_WIDTH, CAPTURE_HEIGHT, RGB);
   pp2 = createImage (CAPTURE_WIDTH, CAPTURE_HEIGHT, RGB);
   pp3 = createImage (CAPTURE_WIDTH, CAPTURE_HEIGHT, RGB);
   pp4 = createImage (CAPTURE_WIDTH, CAPTURE_HEIGHT, RGB);
   frameRate(15);
   counter = 0; 
  
  } 

public void draw() {
  if (mode == true ) { 
    inc = TWO_PI / height * numStripes;
        
    if (counter == 1) { 

       if (folderPath == null) {
         println("No folder was selected...");
         mode = false;
         counter = 0;
       } else {

       //   if(Img.available()) { 
       //   pp4.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);

       PImage frame = Img.getFrame();//Img.read();
       pp4.copy (frame,0,0,frame.width,frame.height,0,0,frame.width,frame.height);

       pp4.updatePixels();

       println ("captured texture");
//          }
       } // folderPath != null
    } // counter == 1
    
    counter++; 

   ////////////////////////////////////////
   //       3 Fringes generation
   // generate the image with the stripes to be
   // projected onto the object that we are scanning

   if (counter >= 2 && counter <= 8) {
      float offset = (counter % 3) * height / numStripes / 3.0 ;
      println(numStripes);
      delay (500);
      for(int y=0; y < height; y++) {
         stroke(sin((y + offset) * inc ) * 128.0 + 128);
         line (0, y, width, y);
      }


   ////////////////////////////////////////
   //       capture the 3 frames needed
   //       to reconstruct the 3d-object

      if (counter == 6 ) {
         delay(1000 / (speed*10+1));
//       if(Img.available()) { 
//          Img.read();
//          pp1.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);

         PImage frame = Img.getFrame();//Img.read();
         pp1.copy (frame,0,0,frame.width,frame.height,0,0,frame.width,frame.height);

         pp1.updatePixels();
    
         println ("captured phase1");    
//       } 
      } // counter==6

      if (counter == 7) {
         delay(1000 / (speed*10+1));   
//       if(Img.available()) { 
//       Img.read();
//       pp2.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);

         PImage frame = Img.getFrame();//Img.read();
         pp2.copy (frame,0,0,frame.width,frame.height,0,0,frame.width,frame.height);

         pp2.updatePixels();   
    
         println ("captured phase2");    
//       } 
      } // counter==7

      if (counter == 8) {
         delay(1000 / (speed*10+1)); 
//       if(Img.available()) { 
//          Img.read();
//          pp3.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);

         PImage frame = Img.getFrame();//Img.read();
         pp3.copy (frame,0,0,frame.width,frame.height,0,0,frame.width,frame.height);
         pp3.updatePixels();

         println ("captured phase3");       
//       } 
      } // counter == 8
    } // counter= 2..8

   if (counter == 9) {
      pp1.save(savePath (folderPath + "/phase1.png"));
      pp2.save(savePath (folderPath + "/phase2.png")); 
      pp3.save(savePath (folderPath + "/phase3.png")); 
      pp4.save(savePath (folderPath + "/tex.png")); 
      mode = false;
      counter  = 0; 
      // white background
      background (255);
      println ("captured and saved images to " + folderPath);
      decodeData();
   }
  } // mode=true (= we are scanning)   
 } // draw()
} // secondApplet
