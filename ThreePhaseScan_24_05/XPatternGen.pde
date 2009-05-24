import processing.video.*;

Capture Img; 
secondApplet s;

float inc;
int counter;


PImage pp1;
PImage pp2;
PImage pp3;
PImage pp4;

boolean mode = false;
boolean reset = false;
float numStripes=32;
String folderPath ;

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


public class secondApplet extends PApplet 
{
 public void setup(){
 
   size(screen.width, screen.height+22);
   background (255);
 
 
   Img = new Capture(this,inputWidth, inputHeight, 15);
   pp1 = createImage (Img.width,Img.height,RGB);
   pp2 = createImage (Img.width,Img.height,RGB);
   pp3 = createImage (Img.width,Img.height,RGB);
   pp4 = createImage (Img.width,Img.height,RGB);
   frameRate(15);
   counter = 0; 
  
  } 

public void draw(){
  if (mode == true ) 
  { 
    inc = TWO_PI / height * numStripes;
        
    if (counter == 1)
    { 
    
    if (folderPath == null) {
   
    println("No folder was selected...");
    mode = false;
    counter = 0;
    } 
    else
    {
    if(Img.available()) { 
    Img.read();
    pp4.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);
    pp4.updatePixels();

    
    println ("captured texture");
    }
   }
 }
    
    counter++; 
    
    
    
    
   if (counter >= 2 && counter <= 8) //  3 Fringes generation
    {
    float offset = (counter % 3) * height / numStripes / 3.0 ;
    println(numStripes);
    delay (500);
    for(int y=0; y < height; y++)
    {
    stroke(sin((y + offset) * inc ) * 128.0 + 128);
    line (0, y, width, y);
    
    }
   if (counter == 6 )
    {
    delay(1000 / (speed*10+1));
    if(Img.available()) { 
    Img.read();
    pp1.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);
    pp1.updatePixels();
    
    println ("captured phase1");    
    } 
    }
   if (counter == 7)
    {
    delay(1000 / (speed*10+1));   
    if(Img.available()) { 
    Img.read();
    pp2.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);
    pp2.updatePixels();   
    
    println ("captured phase2");    
    } 
    }
   if (counter == 8)
    {
    delay(1000 / (speed*10+1)); 
    if(Img.available()) { 
    Img.read();
    pp3.copy (Img,0,0,Img.width,Img.height,0,0,Img.width,Img.height);
    pp3.updatePixels();
    
    println ("captured phase3");       
    } 
    }
    }
   if (counter == 9)
    {
     pp1.save(savePath (folderPath + "/phase1.png"));
     pp2.save(savePath (folderPath + "/phase2.png")); 
     pp3.save(savePath (folderPath + "/phase3.png")); 
     pp4.save(savePath (folderPath + "/tex.png")); 
     mode = false;
     counter  = 0; 
     background (255); 
     println ("captured and saved images to " + folderPath);
     decodeData();   
     }
  }   
 }  
}   
