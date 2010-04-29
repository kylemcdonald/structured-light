Vector trackedFrames, trackedFeatures;

class Frame {
  Vector3D position;
  Rotation rotation;
  float fov;
  
  Frame(float[] position, float[] euler, float fov) {
    this.position = new Vector3D(position[0], position[1], position[2]);
    this.rotation = new Rotation(RotationOrder.ZYX, euler[0], euler[2], euler[1]);
    this.fov = fov;
  }
}

Vector loadFrames(String filename) {
  Vector frames = new Vector();
  String[] file = loadStrings(filename);
  for(int i = 0; i < file.length; i++) {
    String[] cur = split(file[i], ' ');
    if(cur[0].equals("Frame")) {
      i++;
      cur = split(file[i], ' ');
      float[] position = {
        float(cur[1]),
        float(cur[2]),
        float(cur[3])};
      i++;
      cur = split(file[i], ' ');
      float[] euler = {
        radians(float(cur[5])),
        radians(float(cur[6])),
        radians(float(cur[7]))};
      i++;
      cur = split(file[i], ' ');
      float fov = float(cur[4]);
      frames.add(new Frame(position, euler, fov));
    }
  }
  return frames;
}

Vector loadFeatures(String filename) {
  Vector features = new Vector();
  String[] file = loadStrings(filename);
  int i = 0;
  int featureCount = 0;
  for(i = 0; i < file.length; i++) {
    String[] cur = split(file[i], ' ');
    if(cur.length > 1 && cur[1].equals("features")) {
      featureCount = int(cur[0]);
      i++;
      break;
    }
  }
  for(int j = 0; j < featureCount; j++) {
    String[] cur = split(file[i + j], ' ');
    if(cur.length > 4) {
      float[] position = {float(cur[2]), float(cur[3]), float(cur[4])};
      features.add(position);
    }
  }
  return features;
}
