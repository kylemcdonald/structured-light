/*
  Use the wrapped phase information,  and propagate it across the boundaries.
  This implementation uses a priority-based propagation algorithm.
  
  Because the algorithm starts in the center and propagates outwards,
  so if you have noise (e.g.: a black background, a shadow) in
  the center, then it may not reconstruct your image.
*/

PriorityQueue toProcess;
long position;

void phaseUnwrap() {
  int startX = inputWidth / 2;
  int startY = inputHeight / 2;

  toProcess = new PriorityQueue();
  toProcess.add(new WrappedPixel(startX, startY, 0, phase[startY][startX]));
  
  while(!toProcess.isEmpty()) {
    WrappedPixel cur = (WrappedPixel) toProcess.poll();
    int x = cur.x;
    int y = cur.y;
    if(process[y][x]) {
      phase[y][x] = cur.phase;
      process[y][x] = false;
      float d = cur.distance;
      float r = phase[y][x];
      if (y > 0)
        phaseUnwrap(x, y-1, d, r);
      if (y < inputHeight-1)
        phaseUnwrap(x, y+1, d, r);
      if (x > 0)
        phaseUnwrap(x-1, y, d, r);
      if (x < inputWidth-1)
        phaseUnwrap(x+1, y, d, r);
    }
  }
}

void phaseUnwrap(int x, int y, float d, float r) {
  if(process[y][x]) {
    float diff = phase[y][x] - (r - (int) r);
    if (diff > .5)
      diff--;
    if (diff < -.5)
      diff++;
    toProcess.add(new WrappedPixel(x, y, d + distance[y][x], r + diff));
  }
}

void makeDepth() {
  for (int y = 0; y < inputHeight; y += renderDetail) {
    float planephase = 0.5 - (y - (inputHeight / 2)) / zskew;
    for (int x = 0; x < inputWidth; x += renderDetail)
      if (!mask[y][x])
        depth[y][x] = (phase[y][x] - planephase) * zscale;
  }
}

class WrappedPixel implements Comparable {
  public int x, y;
  public float distance, phase;
  WrappedPixel(int x, int y, float distance, float phase) {
    this.x = x;
    this.y = y;
    this.distance = distance;
    this.phase = phase;
  }
  int compareTo(Object o) {
    if(o instanceof WrappedPixel) {
      WrappedPixel w = (WrappedPixel) o;
      if(w.distance == distance)
        return 0;
      if(w.distance < distance)
        return 1;
      else
        return -1;
    } else
      return 0;
  }
}
