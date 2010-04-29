import controlP5.*;
import processing.video.*;

boolean updateMesh = false;

public int resolution = 1;
int curResolution = resolution;

float pitch, yaw, roll;
float maxOffset = 1280;
public int rotateModel = 0;
public int depthRange = 0;
public float modelScale = 1;
public float xOffset = 0;
public float yOffset = 0;
public float zOffset = 0;
public float pitchOffset = 0;
public float yawOffset = 0;
public float rollOffset = 0;
public float overallSpeed = .01;
public float pitchSpeed = 0;
public float yawSpeed = 0;
public float rollSpeed = 0;

public boolean playSequence = false;
int lastFrameNumber = 0;
public int frameNumber = 0;
int frameNumberStart = 0;
int frameNumberStop = 0;
boolean finished = false;
public float verticalFov = degrees(PI / 3.);

float maxCamera = zFar / 4;
float maxTarget = maxCamera;
public int cameraChoice = 0;
public float xCamera = 0;
public float yCamera = 0;
public float zCamera = -maxCamera / 4;
public float xTarget = 0;
public float yTarget = 0;
public float zTarget = 0;

boolean useTracking = false;
boolean trackingLoaded = false;
public boolean trackingOffset = true;
public boolean showFeatures = false;
public boolean showTracking = false;
public boolean showOrientation = true;
public int sourceFps = 0;

public boolean render = false;
public boolean usePng = false;
public boolean useMov = true;

ControlWindow cw;
ControlP5 control;

void setupControls() {  
  control = new ControlP5(this);
  cw = control.addControlWindow("Control Window", 10, 10, 360, 500);
  cw.hideCoordinates();
  cw.setTitle("Controls");

  int y = 0;
  
  control.addTextlabel("playbackControl", "Playback", 4, y += 10).setWindow(cw);
  control.addTextlabel("rotateControl", "Rotate", 84, y).setWindow(cw);
  control.addTextlabel("depthControl", "Depth Range", 164, y).setWindow(cw);
  
  control.addToggle("playSequence", playSequence, 10, y += 10, 9, 9).setWindow(cw);
    
  Radio r = control.addRadio("rotateModel", 90, y);
  r.setWindow(cw);
  r.add("none", 0);
  r.add("+90", 1);
  r.add("-90", 2);
    
  r = control.addRadio("depthRange", 170, y);
  r.setWindow(cw);
  r.add("+/-480", 0);
  r.add("+/-960", 1);
  y += 35;
  
  Bang cur;
  control.addSlider("frameNumber", 0, 0, frameNumber, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetSequence", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  y += 10;
  control.addTextlabel("modelControl", "Model", 4, y += 10).setWindow(cw);
  
  control.addSlider("resolution", 1, 40, resolution, 10, y += 10, 256, 9).setWindow(cw);
  control.addSlider("modelScale", 0, 4, modelScale, 10, y += 10, 256, 9).setWindow(cw);
  
  control.addSlider("xOffset", -maxOffset, maxOffset, xOffset, 20, y += 12, 256, 9).setWindow(cw);
  cur = control.addBang("resetXOffset", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("yOffset", -maxOffset, maxOffset, yOffset, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetYOffset", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("zOffset", -maxOffset, maxOffset, zOffset, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetZOffset", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("yawOffset", -1, 1, yawOffset, 20, y += 12, 256, 9).setWindow(cw);
  cur = control.addBang("resetYawOffset", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("pitchOffset", -1, 1, pitchOffset, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetPitchOffset", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("rollOffset", -1, 1, rollOffset, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetRollOffset", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("yawSpeed", -1, 1, yawSpeed, 20, y += 12, 256, 9).setWindow(cw);
  cur = control.addBang("resetYaw", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("pitchSpeed", -1, 1, pitchSpeed, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetPitch", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("rollSpeed", -1, 1, rollSpeed, 20, y += 10, 256, 9).setWindow(cw);
  cur = control.addBang("resetRoll", 10, y, 9, 9);
  cur.setWindow(cw);
  cur.setLabel("");
  
  control.addSlider("overallSpeed", 0, .1, overallSpeed, 10, y += 10, 256, 9).setWindow(cw);
  
  y += 10;
  control.addTextlabel("cameraControl", "Camera", 4, y += 10).setWindow(cw);
  
  r = control.addRadio("cameraChoice", 10, y += 10);
  r.setWindow(cw);
  r.add("mouse", 0);
  r.add("manual", 1);
  r.add("tracked", 2);
  
  control.addToggle("showOrientation", showOrientation, 10 + 80, y, 9, 9).setWindow(cw);
  
  y += 35;
  
  control.addSlider("verticalFov", 0, 180, verticalFov, 10, y += 10, 256, 9).setWindow(cw);
  control.addSlider("xCamera", -maxCamera, maxCamera, xCamera, 10, y += 12, 256, 9).setWindow(cw);
  control.addSlider("yCamera", -maxCamera, maxCamera, yCamera, 10, y += 10, 256, 9).setWindow(cw);
  control.addSlider("zCamera", -maxCamera, maxCamera, zCamera, 10, y += 10, 256, 9).setWindow(cw);
  control.addSlider("xTarget", -maxTarget, maxTarget, xTarget, 10, y += 12, 256, 9).setWindow(cw);
  control.addSlider("yTarget", -maxTarget, maxTarget, yTarget, 10, y += 10, 256, 9).setWindow(cw);
  control.addSlider("zTarget", -maxTarget, maxTarget, zTarget, 10, y += 10, 256, 9).setWindow(cw);
  
  y += 10;
  control.addTextlabel("trackingControl", "Tracking", 4, y += 10).setWindow(cw);
  
  control.addBang("loadTracking", 10, y += 10, 9, 9).setWindow(cw);
  control.addToggle("showFeatures", showFeatures, 90, y, 9, 9).setWindow(cw);
  
  control.addTextlabel("sourceFpsControl", "Source FPS", 164, y).setWindow(cw);
  r = control.addRadio("sourceFps", 170, y + 10);
  r.setWindow(cw);
  r.add("59.94", 0);
  r.add("23.97", 1);
  
  control.addToggle("trackingOffset", trackingOffset, 10, y += 25, 9, 9).setWindow(cw);
  control.addToggle("showTracking", showTracking, 90, y, 9, 9).setWindow(cw);
  
  y += 10;
  control.addTextlabel("exportControl", "Export", 4, y += 20).setWindow(cw);
  
  control.addToggle("render", render, 10, y += 10, 9, 9).setWindow(cw);
  control.addToggle("usePng", usePng, 10 + 40, y, 9, 9).setWindow(cw);
  control.addToggle("useMov", useMov, 10 + 80, y, 9, 9).setWindow(cw);
  
  control.setAutoDraw(false);
}

public void resetXOffset() {
  control.controller("xOffset").setValue(0);
}

public void resetYOffset() {
  control.controller("yOffset").setValue(0);
}

public void resetZOffset() {
  control.controller("zOffset").setValue(0);
}

public void resetPitchOffset() {
  control.controller("pitchOffset").setValue(0);
  pitch = 0;
}

public void resetYawOffset() {
  control.controller("yawOffset").setValue(0);
  yaw = 0;
}

public void resetRollOffset() {
  control.controller("rollOffset").setValue(0);
  roll = 0;
}

public void resetPitch() {
  control.controller("pitchSpeed").setValue(0);
  pitch = 0;
}

public void resetYaw() {
  control.controller("yawSpeed").setValue(0);
  yaw = 0;
}

public void resetRoll() {
  control.controller("rollSpeed").setValue(0);
  roll = 0;
}

public void resetSequence() {
  control.controller("frameNumber").setValue(frameNumberStart);
}

public void loadTracking(boolean newValue) {
  trackedFrames = loadFrames(inputPath + "/tracking.txt");
  trackedFeatures = loadFeatures(inputPath + "/tracking.txt");
  trackingLoaded = true;
}

public void render(boolean newRender) {
  if(newRender) {
    ((Toggle) control.controller("showOrientation")).setState(false);
    resetSequence();
    if(useMov)
      mm = new MovieMaker(this, width, height, dataPath(inputPath) + "/render.mov", 60, MovieMaker.MOTION_JPEG_A, MovieMaker.LOSSLESS);
  }
  render = newRender;
}

void nextFrame() {
  if(frameNumber + 1 <= frameNumberStop) {
    debug("moving on to " + (frameNumber + 1));
    control.controller("frameNumber").setValue(frameNumber + 1);
  } else {
    finished = true;
  }
}

void drawGui() {
  pushStyle();
  control.draw();
  popStyle();
}
