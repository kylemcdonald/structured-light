import processing.core.*;
import processing.opengl.*;
import javax.media.opengl.*;
import java.nio.*;

int trianglesPerPoint = 2;
int verticesPerTriangle = 3;
int coordinatesPerVertex = 3;
int bytesPerFloat = Float.SIZE / 8;
int bytesPerColor = 4 * Byte.SIZE / 8; // could we take it down to RGB?

public class SuperTri {
  private PApplet p;

  private int capacity;

  private FloatBuffer v;
  private ByteBuffer c;

  private int pointi, coli, numTris;
  private float[] points;
  private byte[] cols;

  private boolean updated;

  public SuperTri(PApplet p, int capacity) {
    if(!(p.g instanceof PGraphicsOpenGL))
      throw new RuntimeException("This library requires OpenGL");
      
    debug("creating SuperTri with capacity " + capacity);
            
    this.p = p;
    this.capacity = capacity;
    
    updated = false;
    
    points = new float[capacity * coordinatesPerVertex];
    cols = new byte[capacity * coordinatesPerVertex * bytesPerColor];
        
    capacity *= 2; // mysterious multiplier
    int maxTriangles = capacity * trianglesPerPoint;
    int maxVertices = maxTriangles * verticesPerTriangle;
    v = ByteBuffer.allocateDirect(maxVertices * coordinatesPerVertex * bytesPerFloat).order(ByteOrder.nativeOrder()).asFloatBuffer();
    c = ByteBuffer.allocateDirect(maxVertices * bytesPerColor).order(ByteOrder.nativeOrder());
    
    clear();
    
    beginGl();
    gl.glEnableClientState(GL.GL_VERTEX_ARRAY);
    gl.glEnableClientState(GL.GL_COLOR_ARRAY);
    endGl();
    
    debug("created SuperTri with " + v.limit() + " vertices, " + c.limit() + " colors");
    int bytes = v.limit() * 4 + c.limit();
    debug("SuperTri using " + ((float) (bytes>>10) / 1024.) + " MB");
  }

  public void clear() {
    debug("clearing SuperTri");
      
    numTris = 0;
    pointi = 0;
    coli = 0;
    v.limit(v.capacity());
    c.limit(c.capacity());
    updated = true;
  }

  public void addPoint(float x, float y, float z, int c) {
    updated = true;
    points[pointi++] = x;
    points[pointi++] = y;
    points[pointi++] = z;
    cols[coli++] = (byte) (c >> 16);
    cols[coli++] = (byte) (c >> 8);
    cols[coli++] = (byte) c;
    cols[coli++] = (byte) 0xff;
  }

  public void addTri(int label1, int label2, int label3) {
    int[] each = {label1, label2, label3};
    for(int i = 0; i < 3; i++) {
      int cur = each[i] * 3;
      v.put(points[cur++]);
      v.put(points[cur++]);
      v.put(points[cur]);
      
      cur = each[i] * 4;
      c.put(cols[cur++]);
      c.put(cols[cur++]);
      c.put(cols[cur++]);
      c.put(cols[cur]);
    }
    numTris++;
    updated = true;
  }
  
  public void draw() {
    if(updated) {
      debug("SuperTri updating with " +
        v.position() + "/" + v.limit() + " (" + (100 * (long) v.position() / v.limit()) + "%) vertices, " +
        c.position() + "/" + c.limit() + " (" + (100 * (long) c.position() / c.limit()) + "%) colors");
      
      v.limit(v.position());
      c.limit(c.position());
      v.rewind();
      c.rewind();
      updated = false;
      
      int bytes = v.limit() * 4 + c.limit(); 
      debug("SuperTri updated, transferring " + ((float) (bytes>>10) / 1024.) + " MB");
      
      gl.glVertexPointer(3, GL.GL_FLOAT, 0, v);
      gl.glColorPointer(4, GL.GL_UNSIGNED_BYTE, 0, c);
    }
    gl.glDrawArrays(GL.GL_TRIANGLES, 0, numTris * 3);
  }
}

