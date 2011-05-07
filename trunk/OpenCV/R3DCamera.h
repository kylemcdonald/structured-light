/* 
 * File:   R3DCamera.h
 * Author: jenicek
 *
 * Created on July 26, 2010, 3:00 PM
 */

#ifndef _R3DCAMERA_H
#define	_R3DCAMERA_H

#include "Global.h"
#include "R3DUtil.h"

class R3DCamera
{
private:
    // 3 vectors specifying camera
    T_GLVector3f vEye;
    T_GLVector3f vLookAt;
    T_GLVector3f vUp;
    // Variables for camera orbital rotation
    GLfloat orbRotX;
    GLfloat orbRotY;
    GLfloat zoom;

public:
    R3DCamera();
    R3DCamera(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat lookAtX, GLfloat lookAtY,
        GLfloat lookAtZ, GLfloat upX, GLfloat upY, GLfloat upZ);
    virtual ~R3DCamera();

    // Class methods
    void RotOrbital(void);
    void Zoom(void);
    void Rotate(float angle, float x, float y, float z);
    void Perspective(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);
    void Place(void);
    

    // Setters
    void SetEye(GLfloat eX, GLfloat eY, GLfloat eZ) { this->vEye = Vec3fSet(eX, eY, eZ); }
    void SetLookAt(GLfloat laX, GLfloat laY, GLfloat laZ) { this->vLookAt = Vec3fSet(laX, laY, laZ); }
    void SetUpVec(GLfloat uX, GLfloat uY, GLfloat uZ) { this->vUp = Vec3fSet(uX, uY, uZ); }
    void SetROrbX(GLfloat angle) { this->orbRotX = angle; }
    void SetROrbY(GLfloat angle) { this->orbRotY = angle; }
    void SetZoom(GLfloat zoom);
    void UpdateROrbX(GLfloat angle) { this->orbRotX += angle; }
    void UpdateROrbY(GLfloat angle) { this->orbRotY += angle; }
    void UpdateZoom(GLfloat zoom);

    // Getters
    T_GLVector3f GetEye(void) { return this->vEye; }
    T_GLVector3f GetLookAt(void) { return this->vLookAt; }
    T_GLVector3f GetUpVec(void) { return this->vUp; }
    GLfloat GetROrbX(void) { return this->orbRotX; }
    GLfloat GetROrbY(void) { return this->orbRotY; }
    GLfloat GetZoom(void) { return this->zoom; }
};

#endif	/* _R3DCAMERA_H */

