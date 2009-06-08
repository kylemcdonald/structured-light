/*
  Use the phase information collected from the phase unwrapping
  and propagate it across the boundaries.
*/
int startX = inputWidth / 2;
int startY = inputHeight / 2;

/**
 * The list of pixels to be processed by propagatePhases()<br/>
 * type: LinkedList<int[2]>
 */
LinkedList toProcess;

void propagatePhases() {

  toProcess = new LinkedList();
  toProcess.add(new int[]{startX, startY});
  _process[startX][startY] = -1;

  while (!toProcess.isEmpty()) {
    int[] xy = (int[]) toProcess.remove();
    int x = xy[0];
    int y = xy[1];
    float r = _wrapphase[y][x];
    
    // propagate in each direction, so long as
    // it isn't masked and it hasn't already been processed
    if (y > 0 && !_mask[y-1][x] && _process[y-1][x]==1) {
      unwrap(r, x, y-1);
    }
    if (y < inputHeight-1 && !_mask[y+1][x] && _process[y+1][x]==1) {
      unwrap(r, x, y+1);
    }
    if (x > 0 && !_mask[y][x-1] && _process[y][x-1]==1) {
      unwrap(r, x-1, y);
    }
    if (x < inputWidth-1 && !_mask[y][x+1] && _process[y][x+1]==1)
      unwrap(r, x+1, y);
    
  }

}

/**
 * TODO: document this global function
 * side-effects:
 * <ul>
 *  <li>_wrapphase[y][x] is upated</li>
 *  <li>_process[y][x] is cleared (set to -1)</li>
 *  <li>new int[]{x, y} is added to toProcess</li>
 * </ul>
 */
void unwrap(float r, int x, int y) {
  // fraction-part behind the decimal comma
  float frac = r - floor(r);
  float myr = _wrapphase[y][x] - frac;
  if (myr > .5) {
    myr--;
  }
  if (myr < -.5) {
    myr++;
  }

  _wrapphase[y][x] = myr + r;
  _process[y][x]   = -1;
  toProcess.add(new int[]{x, y});
}
