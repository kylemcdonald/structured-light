class Camera extends Ray3D {
  float hAov, vAov;
  Camera(Vec3D o, Vec3D d, float hAov, float vAov) {
    super(o, d);
    this.hAov = hAov;
    this.vAov = vAov;
  }
  // not really, but close enough.
  Ray3D getLine(float xi, float yi) {
    Vec3D dir = getDirection();
    float theta = map(xi, 0, 1, -hAov, hAov) / 2;
    float phi = map(yi, 0, 1, -vAov, vAov) / 2;
    float x = sin(theta);
    float y = sin(phi);
    float z = sin(HALF_PI - theta);
    Vec3D lineDir = new Vec3D(x, y, z);
    lineDir.rotateX(getDirection().angleBetween(new Vec3D(0, 0, 1)));
    return new Ray3D(this, lineDir);
  }
  void draw(float imageDist) {    
    // center line
    line(this, imageDist);
    
    // fov
    Ray3D topLeft = getLine(0, 0);
    Ray3D topRight = getLine(1, 0);
    Ray3D bottomLeft = getLine(0, 1);
    Ray3D bottomRight = getLine(1, 1);
    
    line(topLeft, imageDist);
    line(topRight, imageDist);
    line(bottomLeft, imageDist);
    line(bottomRight, imageDist);
    
    beginShape();
    vertex(topLeft.getPointAtDistance(imageDist));
    vertex(topRight.getPointAtDistance(imageDist));
    vertex(bottomRight.getPointAtDistance(imageDist));
    vertex(bottomLeft.getPointAtDistance(imageDist));
    endShape(CLOSE);
    
    pushMatrix();
    translate(this);
    noFill();
    box(10);
    popMatrix();
  }
}
