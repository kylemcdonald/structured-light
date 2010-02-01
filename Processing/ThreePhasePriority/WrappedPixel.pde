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
