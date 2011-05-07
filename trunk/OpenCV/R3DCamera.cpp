/* 
 * File:   R3DCamera.cpp
 * Author: jenicek
 * 
 * Created on July 26, 2010, 3:00 PM
 */

#include "R3DCamera.h"

R3DCamera::R3DCamera()
{
    this->orbRotX = 0.0f;
    this->orbRotY = 0.0f;

    this->vEye = Vec3fSet(0.0f, 0.0f, 0.0f);
    this->vLookAt = Vec3fSet(0.0f, 0.0f, 0.0f);
    this->vUp = Vec3fSet(0.0f, 0.0f, 0.0f);
}

R3DCamera::R3DCamera(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat lookAtX, GLfloat lookAtY,
        GLfloat lookAtZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
    this->orbRotX = 0.0f;
    this->orbRotY = 0.0f;;
    
    this->vEye = Vec3fSet(eyeX, eyeY, eyeZ);
    this->vLookAt = Vec3fSet(lookAtX, lookAtY, lookAtZ);
    this->vUp = Vec3fSet(upX, upY, upZ);
}

R3DCamera::~R3DCamera()
{
}

void R3DCamera::Perspective(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    // Set perspective using glut function
    gluPerspective(fov, aspect, zNear, zFar);
}

void R3DCamera::Place(void)
{ 
    // Set up the camera somewhere in space using glut function
    gluLookAt(this->vEye.x, this->vEye.y, this->vEye.z, this->vLookAt.x, this->vLookAt.y, this->vLookAt.z,
            this->vUp.x, this->vUp.y, this->vUp.z);
}

void R3DCamera::RotOrbital(void)
{
    // Translate to the origin to make the rotation arround origin
    glTranslatef(-this->vEye.x, -this->vEye.y, -this->vEye.z);
    // Arround y axis
    glRotatef(this->orbRotY, 0.0, 1.0, 0.0);
   
    // Translate back to the viewving position
    glTranslatef(this->vEye.x, this->vEye.y, this->vEye.z);
}

void R3DCamera::SetZoom(GLfloat zoom)
{
    this->zoom = zoom;

    if(this->zoom > 3000.0f)
        this->zoom = 3000.0f;
    else if(this->zoom < 0.0f)
        this->zoom = 0.0f;
}

void R3DCamera::UpdateZoom(GLfloat zoom)
{
    this->zoom += zoom;

    if(this->zoom > 3000.0f)
        this->zoom = 3000.0f;
    else if(this->zoom < 0.0f)
        this->zoom = 0.0f;
}

void R3DCamera::Rotate(float angle, float x, float y, float z)
{
    // Get direction vector
    T_GLVector3f vecDir, vecNew;
    Vec3fSub(&vecDir, this->vLookAt, this->vEye);

    // Compute sin and cos of the angle
    float sinAng = (float)sin(angle);
    float cosAng = (float)cos(angle);

    // Compute new x position
    vecNew.x = (cosAng + (1 - cosAng) * x * x) * vecDir.x;
    vecNew.x += ((1 - cosAng) * x * y - z * sinAng) * vecDir.y;
    vecNew.x += ((1 - cosAng) * x * z + y * sinAng) * vecDir.z;
    // Compute new y position
    vecNew.y = ((1 - cosAng) * x * y + z * sinAng) * vecDir.x;
    vecNew.y += (cosAng + (1 - cosAng) * y * y) * vecDir.y;
    vecNew.z += ((1 - cosAng) * y * z - x * sinAng) * vecDir.z;
    // Compute new z position
    vecNew.z = ((1 - cosAng) * x * z - y * sinAng) * vecDir.x;
    vecNew.z += ((1 - cosAng) * y * z + x * sinAng) * vecDir.y;
    vecNew.z += (cosAng + (1 - cosAng) * z * z) * vecDir.z;

    // Compute new rotated look at vector
    Vec3fAdd(&this->vLookAt, this->vEye, vecNew);
}

void R3DCamera::Zoom(void)
{
    // Simply use translating along x and y axis to simulate zoom
    // Must be done before rotations
    glTranslatef(this->zoom, this->zoom / 2, -this->zoom / 6);
}