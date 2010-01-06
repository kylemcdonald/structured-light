// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline3d/

double eps = 100 * Double.MIN_VALUE;

Vec3D rayRayAverage(Ray3D a, Ray3D b) {
  Vec3D p1 = a.getPointAtDistance(0);
  Vec3D p2 = a.getPointAtDistance(1);
  Vec3D p3 = b.getPointAtDistance(0);
  Vec3D p4 = b.getPointAtDistance(1);
  Vec3D p13 = new Vec3D();
  Vec3D p43 = new Vec3D();
  Vec3D p21 = new Vec3D();
  double d1343, d4321, d1321, d4343, d2121;
  double numer, denom;

  p13.x = p1.x - p3.x;
  p13.y = p1.y - p3.y;
  p13.z = p1.z - p3.z;
  p43.x = p4.x - p3.x;
  p43.y = p4.y - p3.y;
  p43.z = p4.z - p3.z;
  if (abs(p43.x) < eps && abs(p43.y) < eps && abs(p43.z) < eps)
    return null;
  p21.x = p2.x - p1.x;
  p21.y = p2.y - p1.y;
  p21.z = p2.z - p1.z;
  if (abs(p21.x) < eps && abs(p21.y) < eps && abs(p21.z) < eps)
    return null;

  d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
  d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
  d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
  d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
  d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

  denom = d2121 * d4343 - d4321 * d4321;
  if (Math.abs(denom) < eps)
    return null;
  numer = d1343 * d4321 - d1321 * d4343;

  double mua = numer / denom;
  double mub = (d1343 + d4321 * (mua)) / d4343;

  float x = (float) (p1.x + mua * p21.x + p3.x + mub * p43.x) / 2;
  float y = (float) (p1.y + mua * p21.y + p3.y + mub * p43.y) / 2;
  float z = (float) (p1.z + mua * p21.z + p3.z + mub * p43.z) / 2;
  
  return new Vec3D(x, y, z);
}


