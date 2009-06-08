/*
  Go through all the pixels in the out of phase images,
  and determine their angle (theta). Throw out noisy pixels.
*/




/**
 * Load the captured images phase[1,2,3].png from disk
 * and populate the global variable _wrapphase with the
 * result of phaseUnwrap(phase1, phase2, phase3)
 */
void phaseUnwrapAll() { 
  if (folderPath == null) {   
     folderPath = (sketchPath + "/data" );
  }
    
  PImage phase1Image = loadImage(folderPath + "/phase1.png");
  PImage phase2Image = loadImage(folderPath + "/phase2.png");
  PImage phase3Image = loadImage(folderPath + "/phase3.png");
  filter(GRAY);
  tex = loadImage(folderPath +"/tex.png");

  println("DEBUG: PhaseUnwrapAll() inputHeight=" + inputHeight);
  println("DEBUG: PhaseUnwrapAll() inputWidth=" + inputWidth);  
  for (int y = 0; y < inputHeight; y++) {
     for (int x = 0; x < inputWidth; x++) {     
        int i = x + y * inputWidth;  
      
        float phase1 = (phase1Image.pixels[i] & 255) / 255.;
        float phase2 = (phase2Image.pixels[i] & 255) / 255.;
        float phase3 = (phase3Image.pixels[i] & 255) / 255.;
      
        float phaseSum   = phase1 + phase2 + phase3;
        float phaseRange = max(phase1, phase2, phase3) - min(phase1, phase2, phase3);

        // avoid the noise floor
        float gamma = phaseRange / phaseSum;
        _mask[y][x] = gamma < noiseR;
        _process[y][x] = 1;
         float theta = phaseUnwrap(phase1, phase2, phase3);
        _wrapphase[y][x] = theta;
     } // x
  } // y
}
