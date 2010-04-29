void drawOrientation() {
  float oa = .9;
  
  gl.glBegin(GL.GL_LINES);
  gl.glColor4f(1., 0., 0., oa);
  gl.glVertex3f(0., 0., 0.);
  gl.glVertex3f(1., 0., 0.);
  
  gl.glColor4f(0., 1., 0., oa);
  gl.glVertex3f(0., 0., 0.);
  gl.glVertex3f(0., 1., 0.);
  
  gl.glColor4f(0., 0., 1., oa);
  gl.glVertex3f(0., 0., 0.);
  gl.glVertex3f(0., 0., 1.);
  gl.glEnd();
}

void drawFeatures() {
  gl.glColor4f(1., 1., 1., .8);
  gl.glPointSize(2);
  gl.glBegin(GL.GL_POINTS);
  for(int i = 0; i < trackedFeatures.size(); i++) {
    float[] cur = (float[]) trackedFeatures.get(i);
    gl.glVertex3f(cur[0], cur[1], cur[2]);
  }
  gl.glEnd();
}

void drawTracking() {
  float ta = .5;
  gl.glBegin(GL.GL_LINES);
  Vector3D curAxis;
  for(int i = 0; i < trackedFrames.size(); i++) {
    Frame cur = (Frame) trackedFrames.get(i);
    float x = (float) cur.position.getX();
    float y = (float) cur.position.getY();
    float z = (float) cur.position.getZ();
    Rotation curRotation = cur.rotation;
    
    gl.glColor4f(1., 0., 0., ta);
    curAxis = curRotation.applyTo(Vector3D.plusI);
    gl.glVertex3f(x, y, z);
    gl.glVertex3d(x + curAxis.getX(), y + curAxis.getY(), z + curAxis.getZ());

    gl.glColor4f(0., 1., 0., ta);
    curAxis = curRotation.applyTo(Vector3D.plusJ);
    gl.glVertex3f(x, y, z);
    gl.glVertex3d(x + curAxis.getX(), y + curAxis.getY(), z + curAxis.getZ());
            
    gl.glColor4f(0., 0., 1., ta);
    curAxis = curRotation.applyTo(Vector3D.plusK);
    gl.glVertex3f(x, y, z);
    gl.glVertex3d(x + curAxis.getX(), y + curAxis.getY(), z + curAxis.getZ());
  }
  gl.glEnd();
}
