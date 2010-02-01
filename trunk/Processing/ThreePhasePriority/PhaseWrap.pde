/*
  Assumes you're using grayscale images.
  Go through all the pixels in the out of phase images,
  and determine their angle (theta). Throw out noisy pixels.
*/

PImage phase1Image, phase2Image, phase3Image;

void loadImages() {
  phase1Image = loadImage("img/phase1.jpg");
  phase2Image = loadImage("img/phase2.jpg");
  phase3Image = loadImage("img/phase3.jpg");
}

void phaseWrap() {
  float sqrt3 = sqrt(3);
  for (int y = 0; y < inputHeight; y++) {
    for (int x = 0; x < inputWidth; x++) {     
      int i = x + y * inputWidth;  
      
      color color1 = phase1Image.pixels[i];
      color color2 = phase2Image.pixels[i];
      color color3 = phase3Image.pixels[i];
      
      float phase1 = brightness(color1) / 255.;
      float phase2 = brightness(color2) / 255.;
      float phase3 = brightness(color3) / 255.;
      
      float phaseRange = max(phase1, phase2, phase3) - min(phase1, phase2, phase3);
      
      mask[y][x] = phaseRange <= noiseThreshold;
      process[y][x] = !mask[y][x];
      distance[y][x] = phaseRange;

      // this equation can be found in Song Zhang's
      // "Recent progresses on real-time 3D shape measurement..."
      // and it is the "bottleneck" of the algorithm
      // it can be sped up with a look up table, which has the benefit
      // of allowing for simultaneous gamma correction.
      phase[y][x] = atan2(sqrt3 * (phase1 - phase3), 2 * phase2 - phase1 - phase3) / TWO_PI;
      
      // build color based on the lightest channels from all three images
      colors[y][x] = blendColor(blendColor(color1, color2, LIGHTEST), color3, LIGHTEST);
    }
  }
  
  for (int y = 1; y < inputHeight - 1; y++) {
    for (int x = 1; x < inputWidth - 1; x++) {
      if(!mask[y][x]) {
        distance[y][x] = (
          diff(phase[y][x], phase[y][x - 1]) +
          diff(phase[y][x], phase[y][x + 1]) +
          diff(phase[y][x], phase[y - 1][x]) +
          diff(phase[y][x], phase[y + 1][x])) / distance[y][x];
      }
    }
  }
}

float diff(float a, float b) {
  float d = a < b ? b - a : a - b;
  return d < .5 ? d : 1 - d;
}
