void line(Ray3D r, float dist) {
  Vec3D p = r.getPointAtDistance(dist);
  line(r.x, r.y, r.z, p.x, p.y, p.z);
}

void line(Vec3D v) {
  line(0, 0, 0, v.x, v.y, v.z);
}

void point(Vec3D v) {
  point(v.x, v.y, v.z);
}

void vertex(Vec3D v) {
  vertex(v.x, v.y, v.z);
}

void translate(Vec3D v) {
  translate(v.x, v.y, v.z);
}
