import processing.opengl.*;
import toxi.geom.*;
import peasy.org.apache.commons.math.geometry.*;

DepthFrame frame;
MovieMaker mm;

float sourcePerTarget = 2.5; // 59.94  / 23.976
float trackingScale = 1. / 128.; // scale everything down

void setup() {
  size(1280, 720, OPENGL);
  
  aspect = (float) width / (float) height;
  
  debugFile = createWriter("debug.txt");
  
  debug("loading control surface");
  setupControls();
  debug("control surface loaded");
      
  glu = new GLU();
  glut = new GLUT();
  
  loadInput(dataPath("../input"));
}

void draw() {  
  background(0);
  
  beginGl();
    
  if(cameraChoice == 2 && trackingLoaded) {
    float trackedFrameNumber = frameNumber;
    if(trackingOffset)
      trackedFrameNumber -= frameNumberStart;
    if(sourceFps == 0)
      trackedFrameNumber /= sourcePerTarget;
    applyTrackedCamera(trackedFrameNumber);
  } else if (cameraChoice == 1)
    applyManualCamera();
  else
    applyMouseCamera();
    
  gl.glDisable(GL.GL_POINT_SMOOTH);
  gl.glDisable(GL.GL_LINE_SMOOTH);
  gl.glDisable(GL.GL_POLYGON_SMOOTH);
  gl.glEnable(GL.GL_DEPTH_TEST);
    
  if(trackingLoaded) {
    boolean usingMouse = cameraChoice == 0;
    if(usingMouse || showFeatures || frameNumber == frameNumberStart)
      drawFeatures();
    if(usingMouse || showTracking)
      drawTracking();
  }
  if(showOrientation)
    drawOrientation();
  
  if(frame != null) {
    if(curResolution != resolution ||
        lastFrameNumber != frameNumber) {
      curResolution = resolution;
      lastFrameNumber = frameNumber;
      frame.load(frameNumber);
      frame.buildMesh();
    }
    
    gl.glScalef(trackingScale, trackingScale, trackingScale);
    frame.drawMesh();
  }
  
  endGl();
  
  if(finished) {
    ((Toggle) control.controller("render")).setState(false);
    finished = false;
  }

  if(render) {
    if(usePng) {
      String renderPath = inputPath + "/render";
      File renderDir = new File(renderPath);
      if(!renderDir.exists()) {
        debug("created directory " + renderPath);
        renderDir.mkdir();
      }
      debug("saving @" + frameNumber + " as .png");
      saveFrame(renderPath + "/" + frameNumber + ".png");
    }
    if(useMov) {
      debug("saving @" + frameNumber + " to .mov");
      mm.addFrame();
    }
    nextFrame();
  } else if(frame != null && playSequence) {
    nextFrame();
  }

  if(!render && mm != null) {
    debug("finishing .mov");
    mm.finish();
    mm = null;
  }

  drawGui();
}

void keyPressed() {
  if(key == 'l') {
    String folderPath = selectFolder();
    if (folderPath != null)
      loadInput(folderPath);
  }
}

boolean leftButton = false;
boolean rightButton = false;
void mousePressed() {
  if(mouseButton == LEFT)
    leftButton = true;
  if(mouseButton == RIGHT)
    rightButton = true;
}

void mouseReleased() {
  if(mouseButton == LEFT)
    leftButton = false;
  if(mouseButton == RIGHT)
    rightButton = false;
}

PrintWriter debugFile;
void debug(String msg) {
  debugFile.println(millis() + "@" + frameCount + "\t" + msg);
  debugFile.flush();
}
