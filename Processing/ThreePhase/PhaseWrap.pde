/*
  Go through all the pixels in the three phase images,
  and determine their wrapped phase. Ignore noisy pixels.
*/

PImage phase1Image, phase2Image, phase3Image;

PImage fitToScreen(PImage img) {
  if(img.width > width)
    img.resize(width, (img.height * width) / img.width);
  else if(img.height > height)
    img.resize((img.width * height) / img.height, height);
  return img;
}

void loadImages() {
  phase1Image = fitToScreen(loadImage("img/phase1.jpg"));
  phase2Image = fitToScreen(loadImage("img/phase2.jpg"));
  phase3Image = fitToScreen(loadImage("img/phase3.jpg"));
}

void phaseWrap() {
  float sqrt3 = sqrt(3);
  for (int y = 0; y < inputHeight; y++) {
    for (int x = 0; x < inputWidth; x++) {     
      int i = x + y * inputWidth;  
      
      color color1 = phase1Image.pixels[i];
      color color2 = phase2Image.pixels[i];
      color color3 = phase3Image.pixels[i];
      
      float phase1 = averageBrightness(color1);
      float phase2 = averageBrightness(color2);
      float phase3 = averageBrightness(color3);
      
      float phaseRange = max(phase1, phase2, phase3) - min(phase1, phase2, phase3);
      
      mask[y][x] = phaseRange <= noiseThreshold;
      ready[y][x] = !mask[y][x];
      distance[y][x] = phaseRange;

      // this equation can be found in Song Zhang's
      // "Recent progresses on real-time 3D shape measurement..."
      // and it is the "bottleneck" of the algorithm
      // it can be sped up with a look up table, which has the benefit
      // of allowing for simultaneous gamma correction.
      phase[y][x] = atan2(sqrt3 * (phase1 - phase3), 2 * phase2 - phase1 - phase3) / TWO_PI;
      
      colors[y][x] = getTexture(color1, color2, color3);
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

float getTexture(float i1, float i2, float i3) {
  return (i1 + i2 + i3 + sqrt(3 * sq(i1 - i3) + sq(2 * i2 - i1 - i3))) / 3;
}

color getTexture(color i1, color i2, color i3) {
  return color(
    getTexture(red(i1), red(i2), red(i3)),
    getTexture(green(i1), green(i2), green(i3)),
    getTexture(blue(i1), blue(i2), blue(i3)));
}

float averageBrightness(color c) {
  return (red(c) + green(c) + blue(c)) / (255 * 3);
}

float diff(float a, float b) {
  float d = a < b ? b - a : a - b;
  return d < .5 ? d : 1 - d;
}
