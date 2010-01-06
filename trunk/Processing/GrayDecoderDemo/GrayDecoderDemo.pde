import toxi.geom.*;
import peasy.*;

PeasyCam cam;

Camera capture, projector;
Vec3D[][] points;

int wi = 640;
int hi = 480;
int n = 18;

float cpDist = 150;
float screenDist = 300;
float pAov = HALF_PI / 8;
float cAov = HALF_PI / 5;

int noiseCutoff = 10;

float[][][] mapping;
void setup() {
  size(640, 480, P3D);
  
  cam = new PeasyCam(this, 512);
  
  projector = new Camera(new Vec3D(0, 0, -screenDist), new Vec3D(0, 0, screenDist), 4 * pAov, 3 * pAov);
  capture = new Camera(new Vec3D(0, -cpDist, -screenDist), new Vec3D(0, cpDist, screenDist), 4 * cAov, 3 * cAov);
  
  mapping = loadGrayPositions();
  
  points = new Vec3D[hi][wi];
  print("Building intersections... ");
  Vec3D last = null;
  for(int i = 0; i < hi; i++) {
    for(int j = 0; j < wi; j++) {
      if(!(mapping[i][j][0] == 0 && mapping[i][j][1] == 0)) {
        Ray3D pLine = projector.getLine(mapping[i][j][1], mapping[i][j][0]);
        Ray3D cLine = capture.getLine((float) j / wi, (float) i / hi);
        points[i][j] = rayRayAverage(pLine, cLine);
        if(points[i][j].distanceTo(last) > noiseCutoff)
          points[i][j] = null;
        last = points[i][j];
      }
    }
  }
  println("done.");
}

void draw() {
  background(0);
  
  stroke(255, 0, 0, 200);
  projector.draw(512);
  
  stroke(0, 0, 255, 200);
  capture.draw(512);
  
  stroke(255);
  noFill();
  for(int i = 0; i < hi; i ++)
    for(int j = 0; j < wi; j ++)
      if(points[i][j] != null)
        point(points[i][j]);
}

void keyPressed() {
  if(key == ' ')
    setup();
}

