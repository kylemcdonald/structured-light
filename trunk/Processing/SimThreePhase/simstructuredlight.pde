/**
 * binarymillenium
 GPL v3.0
November 2009
 
toxi geom library
http://toxiclibs.googlecode.com/files/toxiclibscore-0014.zip

For use with structured light:

http://code.google.com/p/structured-light/
*/

import javax.media.opengl.*;
import processing.opengl.*;
import com.sun.opengl.util.texture.*;  
import toxi.geom.*;

GL gl;

Texture tex[] = new Texture[3];
float rotx = PI/4;
float roty = PI/4;

String imnames[] = new String[3]; 
int imind = 0;



Matrix4x4 view;
/// the position and orientation of the projector
Matrix4x4 projector;
Vec3D projPos;


Vec3D vs[];
int faces[][];

//////////////////////////////////////////////////////////
/// utility functions

Matrix4x4 rotateAbs(Matrix4x4 rot, float df, Vec3D axis) {
  Quaternion quat = new Quaternion(cos(df/2), 
                          new Vec3D(axis.x*sin(df/2),
                                    axis.y*sin(df/2),
                                    axis.z*sin(df/2)) );
                                    
  rot = rot.multiply(quat.getMatrix());
  return rot;
}


///////////////////
Matrix4x4 rotateRel(Matrix4x4 rot, float df, Vec3D axis) {
  Quaternion quat = new Quaternion(cos(df/2), 
                          new Vec3D(axis.x*sin(df/2),
                                    axis.y*sin(df/2),
                                    axis.z*sin(df/2)) );
                                    
  rot = quat.getMatrix().multiply(rot);
  return rot;
}
    
/////////////////////////////////////////////////////////////
void setup() 
{
  size(480, 640, OPENGL);
  perspective(PI/16, float(width)/float(height), 1, 20000);
  
  imnames[0] = "i1.png";
  imnames[1] = "i2.png";
  imnames[2] = "i3.png";
  
  float sc = 1;
  
  int mx = 28;
  vs = new Vec3D[mx*mx];
  faces = new int[(mx-1)*(mx)][4];
  for (int i = 0; i < mx; i++) {
     for (int j = 0; j < mx; j++) {
       float lat = i/(float)(mx-1)*PI-PI/2;
       float lng = j/(float)(mx-1)*PI*2-PI;
       
       int ind = i*mx+j;
       float r = 1.5+1.0*noise(i/12.0,j/12.0);
       vs[ind] = new Vec3D( r*cos(lat)*cos(lng), r*cos(lat)*sin(lng),  r*sin(lat) ); 
       //vs[ind] = vs[ind].scale(10);
      
      if ((j>0) && (i >0)) {
        int find = (i-1)*mx + (j-1);
        //faces[find] = new int[4];
        
        faces[find][0] = ind;
        faces[find][1] = ind-1;
        faces[find][2] = ind-1-mx;
        faces[find][3] = ind - mx;
      }
  }}
  
  gl=((PGraphicsOpenGL)g).gl;
   
  for (int i = 0; i < 3; i++) { 
    try { 
      tex[i] = TextureIO.newTexture(new File(dataPath(imnames[i])),true); 
    }
    catch(Exception e) { println(e); } 
    
    tex[i].setTexParameteri(GL.GL_TEXTURE_WRAP_R,GL.GL_REPEAT);    
    tex[i].setTexParameteri(GL.GL_TEXTURE_WRAP_S,GL.GL_REPEAT);
    tex[i].setTexParameteri(GL.GL_TEXTURE_WRAP_T,GL.GL_REPEAT);
  }
 
  textureMode(NORMALIZED);
  fill(255);
  stroke(color(44,48,32));
  
  
  float angle = PI/2;
  projPos = new Vec3D(0,0,0);
  
  /// point at the origin
  //Vec3D dirV = new Vec3D(0,0,0).sub(projPos).getNormalized();
  //Quaternion dir = new Quaternion( cos(angle/2), 
   //                   new Vec3D(dirV.x*sin(angle/2),dirV.y*sin(angle/2),dirV.z*sin(angle/2)));
                      
  //projector= dir.getMatrix();
  projector = new Matrix4x4(1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,1);
  view = new Matrix4x4(1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,1);
  
}

//////////////////////////////////////////////////////////////////
void keyPressed() {
  float sc = 0.1;
  /*
  if (key == 'a') {
    projPos = projPos.add(sc,0,0);
  }  
  if (key == 'd') {
    projPos = projPos.sub(0.5*sc,0,0);
  }  
  if (key == 'q') {
    projPos = projPos.add(0,0,sc);
  }  
  if (key == 'z') {
    projPos = projPos.sub(0,0,0.5*sc);
  }  
  if (key == 'w') {
    projPos = projPos.add(0,sc,0);
  }  
  if (key == 's') {
    projPos = projPos.sub(0,0.5*sc,0);
  }  
  */
  
  if (key == 'o') {
     texScale *= 1.05; 
  }
  if (key == 'l') {
    texScale *= 0.98;
  }
  /// reset
  if (key == 'r') {
    projPos = new Vec3D(0,0,0);
    projector = new Matrix4x4(1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,1);
    view = new Matrix4x4(1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         0,0,0,1);
                            
    rotx = PI/4;
    roty = PI/4;
    
    texScale = 0.14;
  }  
  
  if (key == 'j') {
    relAngle+= PI/900;
    println("angle " + relAngle*180/PI);
    
    //projector = rotateRel(projector, PI/900, new Vec3D(0,1,0));
  }
  if (key == 'k') {
    relAngle -= PI/1800;
    println("angle " + relAngle*180/PI);
    //projector=rotateAbs(projector, -PI/1800, new Vec3D(0,1,0));
  }
  /*
  if (key == 'u') {
    projector = rotateAbs(projector, sc*PI/10, new Vec3D(0,0,1));
  }
  if (key == 'm') {
    projector = rotateAbs(projector, -sc*PI/20, new Vec3D(0,0,1));
  }
  if (key == 'o') {
    projector = rotateAbs(projector, sc*PI/10, new Vec3D(1,0,0));
  }
  if (key == 'l') {
    projector = rotateAbs(projector, -sc*PI/20, new Vec3D(1,0,0));
  }
  */
  if (key == 'h') {
    drawLine = !drawLine; 
  }
  
  if (key == 'y') {
    switchTex = true;
  }
  
  if (key == 'g') {
    saveIm = true;
  }
  
}

///////////////////////////////////////////////

void apply(Matrix4x4 m) {
 applyMatrix( (float)m.matrix[0][0], (float)m.matrix[0][1], (float)m.matrix[0][2], 0,  
                 (float)m.matrix[1][0], (float)m.matrix[1][1], (float)m.matrix[1][2], 0,  
                 (float)m.matrix[2][0], (float)m.matrix[2][1], (float)m.matrix[2][2], 0,  
                 (float)m.matrix[3][0], (float)m.matrix[3][1], (float)m.matrix[3][2], 1  ); 
            
}

/////

boolean switchTex = false;
boolean drawLine = true;
boolean saveIm = false;
float relAngle = -25.0/180.0*PI;

//////////////////////////////////////////////////////////////
void draw() {
  
  /// only do this synchronously with drawing
  if (switchTex) {
    imind++;
    imind %= 3;  
    /*
    try { tex  = TextureIO.newTexture(new File(dataPath(imnames[imind])),true); }
    catch(Exception e) { println(e); } 
    */
    println("using texture " + imnames[imind]); 
    
  }
  switchTex = false;
  
  background(0);
  noStroke();
  
  translate(width/2.0, height/2.0, -2400);
  
  
  rotateZ(PI/2); 
  //println(relAngle);
  projector=rotateRel(view, relAngle, new Vec3D(0,1,0));
  //Matrix4x4 view = rotateRel(projector, relAngle, new Vec3D(0,1,0) );

  apply(view);
  /*rotateZ(PI/2); 
  rotateY(-rotx);     
  rotateX(roty);
  */

  scale(90);
  drawObject(tex[imind]);
  
  if (saveIm) {
    String name = "phase" + (imind + 1) + ".jpg";
    saveFrame(name);
    println("saving " + name);
    
    imind++;
    
  } 
  if (imind > 2) { saveIm = false;
    imind = 0;
  }
  
  
  if (drawLine) {
    stroke(255,255,255);
    line(projPos.x,projPos.y, projPos.z, 
         projPos.x+15.0*(float)projector.matrix[2][0],
         projPos.y+15.0*(float)projector.matrix[2][1], 
         projPos.z+15.0*(float)projector.matrix[2][2]);
  }
  
 
}

///////////////////////////////////////////////////

float texScale = 0.14;
void vertexProj(Vec3D v,boolean verbose) {
  
  Vec3D rel = v.sub(projPos);

  Vec3D uv = projector.apply(rel);
  
  uv = uv.scale(texScale);
  
  //vertex(v.x, v.y, v.z, pt.x*10, pt.y *10);
  gl.glTexCoord2f(uv.x,uv.y);
  gl.glVertex3f(v.x,v.y,v.z);
  
  //vertex(v.x,v.y,v.z);//, uv.x,uv.y);
  
  if (verbose) println(uv.x + " " + uv.y);
}

void vertexProj(Vec3D v) {
  vertexProj(v,false);
}

/// draw an object with texture projected onto it
void drawObject(Texture tex) {
  
  /*
  fill(255);
  stroke(255);
  beginShape(LINES);
  for (int i = 0; i < faces.length; i++) {
    vertexProj(vs[faces[i][0]]);
    vertexProj(vs[faces[i][1]]);
    vertexProj(vs[faces[i][2]]);
    vertexProj(vs[faces[i][3]]);
    
    println(vs[faces[i][0]]);
  }
  endShape();
  println();
  */
  
  if (true) {
  PGraphicsOpenGL pgl = (PGraphicsOpenGL) g;  
  GL gl = pgl.beginGL();  
   
  tex.bind();  
  tex.enable();  
   
  // beginShape(QUADS);
  // texture(tex);
 
  gl.glBegin(GL.GL_QUADS);
  //gl.glNormal3f( 0.0f, 0.0f, 1.0f); 
  
  gl.glTexParameteri( GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, GL.GL_REPEAT );
  gl.glTexParameteri( GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, GL.GL_REPEAT );
  gl.glTexParameteri( GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_R, GL.GL_REPEAT );
  
  // Given one texture and six faces, we can easily set up the uv coordinates
  // such that four of the faces tile "perfectly" along either u or v, but the other
  // two faces cannot be so aligned.  This code tiles "along" u, "around" the X/Z faces
  // and fudges the Y faces - the Y faces are arbitrarily aligned such that a
  // rotation along the X axis will put the "top" of either texture at the "top"
  // of the screen, but is not otherwised aligned with the X/Z faces. (This
  // just affects what type of symmetry is required if you need seamless
  // tiling all the way around the cube)
  
  // +Z "front" face
  for (int i = 0; i < faces.length; i++) {
    vertexProj(vs[faces[i][0]]);
    vertexProj(vs[faces[i][1]]);
    vertexProj(vs[faces[i][2]]);
    vertexProj(vs[faces[i][3]]);
  }
  
  
  
  gl.glEnd();
  //println();
  //endShape();

  tex.disable();
  pgl.endGL();
  
  }
}

void mouseDragged() {
  float rate = 0.005;
  rotx = (pmouseY-mouseY) * rate;
  roty = (mouseX-pmouseX) * rate;
  
  //projector = rotateRel(projector, -rotx, new Vec3D(0,1,0));
  //projector = rotateRel(projector, roty, new Vec3D(1,0,0));
  
  view = rotateRel(view, -rotx, new Vec3D(0,1,0));
  view = rotateRel(view, roty, new Vec3D(1,0,0));
}
