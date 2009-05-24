// Adapted by Kyle Mc Donald from Florian Jennet, from Alex Evans, from Song Zhang.
// Extended version by Gunter Weber


import peasy.*;

float zscale = 26;        
float zskew = 10;
float noiseR = 0.1; // smaller (even negative) >>> more noise ; set too high above 0.28 >>> nothing visible 

int inputWidth = 640;
int inputHeight = 480;

PeasyCam cam;
PImage tex;





void setup() {
  size(800, 600, P3D);
  frame.setResizable(true);
  cam = new PeasyCam(this, width);
  PFrame f = new PFrame();
  fill(255);
  noStroke();
  
  controller();
  
  decodeData();
  
  
}

void draw () {
  
  background(0);
  controlP5.draw();
  if (light == true)
    {
    directionalLight(108, 102, 126, .5, 0, -1);
    emissive(60,30,0);
    ambient(51, 26, 0);
    }
  
  int step = 2;  
  for (int y = step; y < inputHeight; y += step) {
    if (viewMode == 1 ){      
      beginShape(TRIANGLE_STRIP);
      fill(255);
      noStroke();
      texture(tex);      
      for (int x = step; x < inputWidth; x += step) {
        float planephase = 0.5 - (y - (inputHeight / 2)) / zskew;
        if (_process[y][x]==-1){          
          vertex(x - (inputWidth / 2), y - (inputHeight / 2), (_wrapphase[y][x] - planephase) * zscale, x, y);
        }
        planephase = 0.5 - (y - step - (inputHeight / 2)) / zskew;  
        if (_process[y - step][x - step]==-1){
          vertex(x - step - (inputWidth / 2), y - step - (inputHeight / 2), (_wrapphase[y - step][x - step] - planephase) * zscale, x, y); 
        }        
      }  
      endShape(CLOSE);
     }
     if (viewMode == 2) {          
        noFill();
        stroke(110,45,15); 
        float planephase = 0.5 - (y - (inputHeight/ 2)) / zskew;
        for (int x = step; x < inputWidth; x += step)
          if (_process[y][x]==-1 && _process[y - step][x - step]==-1)
            point(x- (inputWidth / 2), y- (inputHeight / 2), (_wrapphase[y][x] - planephase) * zscale);
     }
     if (viewMode == 3 ){
        
       light = true; 
       beginShape(TRIANGLE_STRIP);
        fill(255,230,130);
        noStroke();
        for (int x = step; x < inputWidth; x += step) {
        float planephase = 0.5 - (y - (inputHeight / 2)) / zskew;
        if (_process[y][x]==-1){          
          vertex(x - (inputWidth / 2), y - (inputHeight / 2), (_wrapphase[y][x] - planephase) * zscale);
        }
        planephase = 0.5 - (y - step - (inputHeight / 2)) / zskew;  
        if (_process[y - step][x - step]==-1){
          vertex(x - step - (inputWidth / 2), y - step - (inputHeight / 2), (_wrapphase[y - step][x - step] - planephase) * zscale); 
        }
      }  
      endShape(CLOSE);
     }        
   }
   if ( showCenter )
   {
      noFill();

        // red == x
        stroke(50);
        beginShape();
        vertex(-1000,0,0);
        vertex(1000,0,0);
        endShape();

        // green == y
        stroke(0);
        beginShape();
        vertex(0,-1000,0);
        vertex(0,1000,0);
        endShape();

        // blue == z
        stroke(255);
        beginShape();
        vertex(0,0,-1000);
        vertex(0,0,1000);
        endShape();
     }
    if ( saveAFrame == true)
     {
        saveFrame("Screenshots/image-"+step+"-####.jpg");
        saveAFrame = false;
     }  
}
