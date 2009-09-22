import peasy.*;

/*
 These three variables are the main "settings".
 
 zscale corresponds to how much "depth" the image has,
 zskew is how "skewed" the imaging plane is.
 
 These two variables are dependent on both the angle
 between the projector and camera, and the number of stripes.
 The sign on both is based on the direction of the stripes
 (whether they're moving up vs down)
 as well as the orientation of the camera and projector
 (which one is above the other).
 
 noiseTolerance can significantly change whether an image
 can be reconstructed or not. Start with it small, and work
 up until you start losing important parts of the image.
*/
float zscale = 140;
float zskew = 23;
float noiseTolerance = 0.15;

int inputWidth = 480;
int inputHeight = 640;

PeasyCam cam;

float[][] phase = new float[inputHeight][inputWidth];
boolean[][] mask = new boolean[inputHeight][inputWidth];
boolean[][] process = new boolean[inputHeight][inputWidth];

void setup() {
  size(inputWidth, inputHeight, P3D);
  cam = new PeasyCam(this, width);
  
  stroke(255);
  
  phaseWrap();
  phaseUnwrap();
}

void draw () {
  background(0);
  translate(-inputWidth / 2, -inputHeight / 2);  
  int step = 2;
  for (int y = step; y < inputHeight; y += step) {
    float planephase = 0.5 - (y - (inputHeight / 2)) / zskew;
    for (int x = step; x < inputWidth; x += step)
      if (!mask[y][x])
        point(x, y, (phase[y][x] - planephase) * zscale);
  }
}
