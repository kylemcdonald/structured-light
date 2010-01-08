
public class XPatternGen
{
   
float inc;

PImage pp[][];
String pnames[][];

String folderPath = "/data" ;

int numPhases = 3;
int numPeriods = 2;
int startPeriodPower = 8;

 XPatternGen() {}
 
/// startPeriodPower needs to be > numPeriods
 XPatternGen(int pwidth, int pheight,int startPeriodPower, int numPhases, int numPeriods){
   setup(pwidth, pheight,startPeriodPower, numPhases, numPeriods);
 }
 
 public void setup(int pwidth, int pheight,int startPeriodPower, int numPhases, int numPeriods) {
   this.numPhases = numPhases;
   this.numPeriods = numPeriods;
   this.startPeriodPower = startPeriodPower;
   pp = new PImage[numPeriods][numPhases];
   pnames = new String[numPeriods][numPhases];
   for (int i = 0; i < numPeriods; i++) {
   for (int j = 0; j < numPhases; j++) {
     pp[i][j] = createImage (pwidth,pheight,RGB);
   }}

  
    draw();
  } 

public void draw() {

    int curPeriod = 2^startPeriodPower;
    
    for (int i = 0; i < numPeriods; i++) {
    for (int j = 0; j < numPhases; j++) {
    
      float offset = (float)curPeriod * (float)(j % numPhases)/(float)numPhases;

    
      for(int y=0; y < height; y++)
      {
        /// it's important that all the phases start from height/2
        /// and not be lined up on the left edge
        color c = color(sin((y-height/2 + offset) * (2*PI/(float)curPeriod) ) * 128.0 + 128);
        for(int x=0; x < width; x++) {
          pp[i][j].pixels[y*width+x] = c;
        } 
       pp[i][j].updatePixels(); 
      }
      
      
           
      /// TBD make saving optional
      pnames[i][j] =   (i+1000) + "phase" + (j+1000) +".png";
      pp[i][j].save(savePath(folderPath + "/" + pnames[i][j]));
    }
    curPeriod = curPeriod/2;
    }
   
    println ("captured and saved images to " + folderPath);
    
 }  
}   
