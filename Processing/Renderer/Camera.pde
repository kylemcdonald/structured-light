import peasy.*;

float aspect;
float zNear = .1;
float zFar = 200;

void applyTrackedCamera(float frameNumber24p) {
  int curFrameNumber = (int) frameNumber24p;
  int nextFrameNumber = curFrameNumber + 1;
  
  Frame curFrame = (Frame) trackedFrames.get(curFrameNumber);
  
  Rotation rot;
  Vector3D pos;
  float fov;
  if(frameNumber24p == curFrameNumber || nextFrameNumber >= trackedFrames.size()) {
    if(render)
      debug("raw tracked camera at " + frameNumber24p);
    rot = curFrame.rotation;
    pos = curFrame.position;
    fov = curFrame.fov;
  } else {
    float t = frameNumber24p - (int) frameNumber24p;
    if(render)
      debug("interpolatied tracked camera at " + nf(t, 0, 3) + " after " + curFrameNumber);
    Frame nextFrame = (Frame) trackedFrames.get(nextFrameNumber);
    rot = InterpolationUtil.slerp(curFrame.rotation, nextFrame.rotation, t);
    pos = InterpolationUtil.linear(curFrame.position, nextFrame.position, t);
    fov = (float) InterpolationUtil.linear(curFrame.fov, nextFrame.fov, t);
  }
  
  Vector3D up = rot.applyTo(Vector3D.plusJ);
  Vector3D look = rot.applyTo(Vector3D.plusK);
  
  gluPerspective(fov, aspect, zNear, zFar);
  gl.glScalef(-1, 1, 1);
  gluLookAt(
    (float) pos.getX(), (float) pos.getY(), (float) pos.getZ(),
    (float) (pos.getX() + look.getX()), (float) (pos.getY() + look.getY()), (float) (pos.getZ() + look.getZ()),
    (float) up.getX(), (float) up.getY(), (float) up.getZ());
}

float rotationScale = .2;
float mouseYaw = 0;
float mousePitch = 0;

void mouseDragged() {
  float x = mouseX - pmouseX;
  float y = mouseY - pmouseY;
  if(leftButton && rightButton) {
    if(cameraChoice == 2) {
      zOffset += y;
      control.controller("zOffset").setValue(zOffset);
    }
  } else if(leftButton) {
    if(cameraChoice == 0) {
      mouseYaw += y;
      mousePitch += x;
    } else if(cameraChoice == 2) {
      yawOffset -= radians(x) * rotationScale;
      pitchOffset -= radians(y) * rotationScale;
      control.controller("yawOffset").setValue(yawOffset);
      control.controller("pitchOffset").setValue(pitchOffset);
    }
  } else if(rightButton) {
    if(cameraChoice == 0) {
      zCamera -= y / 32;
      control.controller("zCamera").setValue(zCamera);
    } else if(cameraChoice == 2) {
      xOffset += x;
      yOffset -= y;
      control.controller("xOffset").setValue(xOffset);
      control.controller("yOffset").setValue(yOffset);
    }
  }
}

void applyMouseCamera() {
  gluPerspective(verticalFov, aspect, zNear, zFar);
  Rotation q = new Rotation(RotationOrder.XYZ,
    radians(mouseYaw) * rotationScale,
    radians(mousePitch) * rotationScale, 0);
  Vector3D pos = q.applyTo(Vector3D.plusK).scalarMultiply(zCamera);
  gl.glScalef(-1, 1, 1);
  Vector3D up = q.applyTo(Vector3D.plusJ);
  gluLookAt(
    (float) pos.getX(), (float) pos.getY(), (float) pos.getZ(),
    0, 0, 0,
    (float) up.getX(), (float) up.getY(), (float) up.getZ());
}

void applyManualCamera() {
  gluPerspective(verticalFov, aspect, zNear, zFar);
  gl.glScalef(-1, 1, 1);
  gluLookAt(
    xCamera, yCamera, zCamera,
    xTarget, yTarget, zTarget,
    0, 1, 0);
}
