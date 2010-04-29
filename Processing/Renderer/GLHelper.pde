import processing.opengl.*;
import com.sun.opengl.util.*;
import javax.media.opengl.glu.*;
import javax.media.opengl.*;
import java.nio.*;

PGraphicsOpenGL pgl;
GL gl;
GLU glu;
GLUT glut;

void beginGl() {
  pgl = (PGraphicsOpenGL) g;
  gl = pgl.beginGL();
}

void endGl() {
  pgl.endGL();
}

void gluPerspective(float fov, float aspect, float zNear, float zFar) {
  gl.glMatrixMode(GL.GL_PROJECTION);
  gl.glLoadIdentity();
  glu.gluPerspective(fov, aspect, zNear, zFar);
}

void gluLookAt(float cx, float cy, float cz, float tx, float ty, float tz, float ux, float uy, float uz) {
  gl.glMatrixMode(GL.GL_MODELVIEW);
  gl.glLoadIdentity();
  glu.gluLookAt(cx, cy, cz, tx, ty, tz, ux, uy, uz);
}
