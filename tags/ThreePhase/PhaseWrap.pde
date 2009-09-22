/*
  Assumes you're using grayscale images.
  Go through all the pixels in the out of phase images,
  and determine their angle (theta). Throw out noisy pixels.
*/

void phaseWrap() { 
  PImage phase1Image = loadImage("phase1.jpg");
  PImage phase2Image = loadImage("phase2.jpg");
  PImage phase3Image = loadImage("phase3.jpg");
  
  float sqrt3 = sqrt(3);
  for (int y = 0; y < inputHeight; y++) {
    for (int x = 0; x < inputWidth; x++) {     
      int i = x + y * inputWidth;  
      
      float phase1 = (phase1Image.pixels[i] & 255) / 255.;
      float phase2 = (phase2Image.pixels[i] & 255) / 255.;
      float phase3 = (phase3Image.pixels[i] & 255) / 255.;
      
      float phaseSum = phase1 + phase2 + phase3;
      float phaseRange = max(phase1, phase2, phase3) - min(phase1, phase2, phase3);
      
      // avoid the noise floor
      float gamma = phaseRange / phaseSum;
      mask[y][x] = gamma < noiseTolerance;
      process[y][x] = !mask[y][x];

      // this equation can be found in Song Zhang's
      // "Recent progresses on real-time 3D shape measurement..."
      // and it is the "bottleneck" of the algorithm
      // it can be sped up with a LUT, which has the benefit
      // of allowing for simultaneous gamma correction.
      phase[y][x] = atan2(sqrt3 * (phase1 - phase3), 2 * phase2 - phase1 - phase3) / TWO_PI;
    }
  }
}
