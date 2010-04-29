String inputPath;
void loadInput(String path) {
  debug("attempting to load " + path);
  File dir = new java.io.File(path + "/depth");
  if(dir.exists()) {
    debug("valid input directory");
    inputPath = path;
    FilenameFilter pngFilter = new FilenameFilter() {
      boolean accept(File dir, String name) {
        return name.toLowerCase().endsWith(".png");
      }
    };
    
    String[] files = dir.list(pngFilter);
    frameNumberStart = Integer.MAX_VALUE;
    frameNumberStop = 0;
    for(int i = 0; i < files.length; i++) {
      int cur = int(split(files[i], '.')[0]);
      if(cur < frameNumberStart)
        frameNumberStart = cur;
      if(cur > frameNumberStop)
        frameNumberStop = cur;
    }

    debug("found frames from " + frameNumberStart + " to " + frameNumberStop);
    control.controller("frameNumber").setMin(frameNumberStart);
    control.controller("frameNumber").setMax(frameNumberStop);
    control.controller("frameNumber").setValue(frameNumberStart);

    trackingLoaded = false;

    frame = new DepthFrame();
    frame.load(frameNumberStart);
    frame.buildMesh();
  } else {
    debug("invalid input directory");
  }
}

