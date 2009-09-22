/*
  Use the wrapped phase information,  and propagate it across the boundaries.
  This implementation uses a flood-fill propagation algorithm.
  
  Because the algorithm starts in the center and propagates outwards,
  so if you have noise (e.g.: a black background, a shadow) in
  the center, then it may not reconstruct your image.
*/

LinkedList toProcess;

void phaseUnwrap() {
  int startX = inputWidth / 2;
  int startY = inputHeight / 2;

  toProcess = new LinkedList();
  toProcess.add(new int[]{startX, startY});
  process[startX][startY] = false;

  while (!toProcess.isEmpty()) {
    int[] xy = (int[]) toProcess.remove();
    int x = xy[0];
    int y = xy[1];
    float r = phase[y][x];
    
    if (y > 0)
      phaseUnwrap(r, x, y-1);
    if (y < inputHeight-1)
      phaseUnwrap(r, x, y+1);
    if (x > 0)
      phaseUnwrap(r, x-1, y);
    if (x < inputWidth-1)
      phaseUnwrap(r, x+1, y);
  }
}

void phaseUnwrap(float basePhase, int x, int y) {
  if(process[y][x]) {
    float diff = phase[y][x] - (basePhase - (int) basePhase);
    if (diff > .5)
      diff--;
    if (diff < -.5)
      diff++;
    phase[y][x] = basePhase + diff;
    process[y][x] = false;
    toProcess.add(new int[]{x, y});
  }
}
