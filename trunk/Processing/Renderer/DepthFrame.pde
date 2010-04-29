PApplet applet = this;
SuperTri mesh;

class DepthFrame {
  PImage depth, tex;
  
  int w, h;
  byte[] z;
  int[] label;
  DepthFrame() {
    debug("allocating space for DepthFrame");
        
    int n = 1280 * 720; // maximum image size
    mesh = new SuperTri(applet, n);
    z = new byte[n];
    label = new int[n];
  }
  void clear() {
    if(mesh != null)
      mesh.clear();
  }
  void load(int frame) {
    debug("loading DepthFrame @" + frame + " with resolution " + curResolution);
    
    depth = loadImage(inputPath + "/depth/" + frame + ".png");
    tex = loadImage(inputPath + "/texture/" + frame + ".png");
    
    if(curResolution != 1) {
      noSmooth();
      depth.resize(depth.width / curResolution, depth.height / curResolution);
      tex.resize(tex.width / curResolution, tex.height / curResolution);
    }
    
    w = depth.width;
    h = depth.height;
    
    debug("image loaded at " + w + " by " + h);
    
    clear();
  }
  void buildMesh() {
    debug("labelling vertices");
    
    int n = w * h;
    int j = 0;
    for(int i = 0; i < n; i++) {
      z[i] = (byte) (depth.pixels[i] & 0xff);
      if(z[i] != 0)
        label[i] = j++;
    }
    
    debug((100 * j / n) + "% image filled");
    
    int i = 0;
    for(int y = 0; y < h; y++) {
      for(int x = 0; x < w; x++) {
        if(z[i] != 0) {
          int curColor = tex.pixels[i];
          mesh.addPoint(
            x, y, depth.pixels[i] & 0xff, curColor);
        }
        i++;
      }
    }
    
    for(int y = 0; y < h - 1; y += 1) {
      for(int x = 0; x < w - 1; x += 1) {
        int nw = y * w + x;
        int ne = nw + 1;
        int sw = nw + w;
        int se = sw + 1;
        if(z[nw] == 0) { // no nw, only one triangle possible
          if(z[sw] != 0 && z[se] != 0 && z[ne] != 0)
            mesh.addTri(label[sw], label[ne], label[se]);
        } else {
          if(z[ne] == 0) { // no ne, only one triangle possible
            if(z[sw] != 0 && z[se] != 0) {
              mesh.addTri(label[nw], label[se], label[sw]);
            }
          } else { // has nw and ne
            if(z[sw] != 0 && z[se] != 0) {
              mesh.addTri(label[nw], label[ne], label[sw]);
              mesh.addTri(label[sw], label[ne], label[se]);
            } else if(z[sw] != 0) {
              mesh.addTri(label[nw], label[ne], label[sw]);
            } else if(z[se] != 0) {
              mesh.addTri(label[nw], label[ne], label[se]);
            }
          }
        }
      }
    }
  }
  void drawMesh() {
    gl.glTranslatef(xOffset, yOffset, zOffset); // position globally
    applyOrientation(); // rotate about center
    if(rotateModel == 1) // apply landscape/portrait fix
      gl.glRotatef(-90, 0, 0, 1);
    else if(rotateModel == 2)
      gl.glRotatef(90, 0, 0, 1);
    gl.glScalef(curResolution, curResolution, -(depthRange == 0 ? 960. : 1920.) / 256.); // account for resolution and depth
    gl.glScalef(modelScale, modelScale, modelScale); // scale from center
    gl.glTranslatef(-w / 2, -h / 2, -128); // center
    mesh.draw();
  }
}

void applyOrientation() {
  yaw += yawSpeed * overallSpeed;
  pitch += pitchSpeed * overallSpeed;
  roll += rollSpeed * overallSpeed;

  Quaternion modelOrientation = Quaternion.createFromEuler(
    yaw + yawOffset * PI,
    roll + rollOffset * PI,
    pitch + pitchOffset * PI);
  float[] aa = modelOrientation.toAxisAngle();
  gl.glRotatef(degrees(aa[0]), aa[1], aa[2], aa[3]);
}
