float[][] _wrapphase = new float[inputHeight][inputWidth];
boolean[][] _mask    = new boolean[inputHeight][inputWidth];
float[][] _process   = new float[inputHeight][inputWidth];

void decodeData() {
  phaseUnwrapAll();
  propagatePhases();  
}
