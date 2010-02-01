import controlP5.*;

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
 
 noiseThreshold can significantly change whether an image
 can be reconstructed or not. Start with it small, and work
 up until you start losing important parts of the image.
 */
 
public float noiseThreshold = 0.1;
public float zscale = 130;
public float zskew = 24; 
public int renderDetail = 2;

ControlWindow controlWindow;
ControlP5 control;

boolean takeScreenshot;
void screenshot() {
  takeScreenshot = true;
}

void setupControls() {
  control = new ControlP5(this);
  controlWindow = control.addControlWindow("controlWindow", 10, 10, 350, 128);
  controlWindow.hideCoordinates();
  controlWindow.setTitle("Decoding Parameters");

  int y = 20;
  control.addSlider("noiseThreshold", 0, 1, noiseThreshold, 10, y += 10, 256, 9).setWindow(controlWindow);
  control.addSlider("zscale", -256, 256, zscale, 10, y += 10, 256, 9).setWindow(controlWindow);
  control.addSlider("zskew", -64, 64, zskew, 10, y += 10, 256, 9).setWindow(controlWindow);
  control.addSlider("renderDetail", 1, 4, renderDetail, 10, y += 10, 256, 9).setWindow(controlWindow);
  control.addBang("screenshot", 10, y += 10, 9, 9).setWindow(controlWindow);
}

void noiseThreshold(float newThreshold) {
  if(newThreshold != noiseThreshold) {
    noiseThreshold = newThreshold;
    update = true;
  }
}
