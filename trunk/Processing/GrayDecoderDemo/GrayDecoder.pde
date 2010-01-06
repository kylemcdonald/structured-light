int cutoff = 100;

float[][][] loadGrayPositions() {
  println("Acquiring encoded position data."); 
  boolean[][][] grayMapping = new boolean[hi][wi][n];
  boolean[][] binImg = new boolean[hi][wi];
  for(int level = 0; level < n; level++) {
    print(level + ": loading... ");
    PImage cur = loadImage((level + 1) + ".jpg");
    
    print("binary... ");
    makeBinary(cur, cutoff, binImg);
    
    print("noting... ");
    for(int i = 0; i < hi; i++) {
      for(int j = 0; j < wi; j++) {
        grayMapping[i][j][level] = binImg[i][j];
      }
    }
    
    println("done.");
  }
  
  print("Converting gray code to binary... ");
  boolean[][][] binMapping = new boolean[hi][wi][n];
  for(int i = 0; i < hi; i++) {
    for(int j = 0; j < wi; j++) {
      boolean lasty = false;
      boolean lastx = false;
      for(int level = 0; level < n; level += 2) {
        binMapping[i][j][level] = grayMapping[i][j][level] ^ lasty;
        lasty = binMapping[i][j][level];
        binMapping[i][j][level + 1] = grayMapping[i][j][level + 1] ^ lastx;
        lastx = binMapping[i][j][level + 1];
      }
    }
  }
  println("done.");
  
  float[] levelOffset = new float[n];
  for(int i = 0; i < n; i++) {
    levelOffset[i] = 1. / pow(2, (i / 2) + 1);
  }
  
  print("Decoding position mapping... ");
  float[][][] mapping = new float[hi][wi][2];
  for(int i = 0; i < hi; i++) {
    for(int j = 0; j < wi; j++) {
      for(int level = 0; level < n; level += 2) {
        mapping[i][j][0] += binMapping[i][j][level] ? levelOffset[level] : 0;
        mapping[i][j][1] += binMapping[i][j][level + 1] ? levelOffset[level + 1] : 0;
      }
      mapping[i][j][0] *= 4./3.; // the entire 1024x1024 isn't squished into 1024x768
    }
  }
  println("done.");
  
  return mapping;
}

boolean[][] makeBinary(PImage img, int cutoff, boolean[][] binImg) {
  img.loadPixels();
  for(int y = 0; y < img.height; y++) {
    int row = y * img.width;
    for(int x = 0; x < img.width; x++) {
      binImg[y][x] = brightness(img.pixels[row + x]) > cutoff;
    }
  }
  return binImg;
}

PImage makeImage(boolean[][] binImg) {
  int wi = binImg[0].length;
  int hi = binImg.length;
  PImage img = new PImage(wi, hi);
  img.loadPixels();
  for(int y = 0; y < hi; y++) {
    int row = y * wi;
    for(int x = 0; x < wi; x++) {
      img.pixels[row + x] = binImg[y][x] ? 0xffffff : 0x000000;
    }
  }
  img.updatePixels();
  return img;
}
