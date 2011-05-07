/* 
 * File:   R3DUtil.h
 * Author: jenicek
 *
 * Created on July 26, 2010, 2:54 PM
 *
 * Utility functions for 3D drawing
 *
 */

#ifndef _R3DUTIL_H
#define	_R3DUTIL_H

#include "Global.h"

#define DEG_TO_RAD(x) ((x) * MATH_PI / 180.0)
#define RAD_TO_DEG(x) ((x) * 180.0 / MATH_PI)

// Structures
// 3D vector structure
typedef struct glVec3f
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
}T_GLVector3f;

// 2D vector structure
typedef struct glVec2f
{
    GLfloat x;
    GLfloat y;
}T_GLVector2f;

// RGBA color structure
typedef struct colRGBA
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
}T_ColorRGBA;

// Set 3D vector
T_GLVector2f Vec2fSet(GLfloat x, GLfloat y);

// Set 3D vector
T_GLVector3f Vec3fSet(GLfloat x, GLfloat y, GLfloat z);

// Set RGBA color
T_ColorRGBA ColorRGBASet(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

// 3D vectors dot product
float Vec3fDotP(T_GLVector3f v1, T_GLVector3f v2);

// 3D vector magnitude
float Vec3fMag(T_GLVector3f v);

// Normalize 3D vector
void Vec3fNormalize(T_GLVector3f *vec);

// 3D vectors cross product
void Vec3fCrossP(T_GLVector3f *vOut, T_GLVector3f v1In, T_GLVector3f v2In);

// Multiply 3D vector by scalar
void Vec3fMultScal(T_GLVector3f *vOut, T_GLVector3f vIn, GLfloat scalar);

// 3D vector addition
void Vec3fAdd(T_GLVector3f *vOut, T_GLVector3f v1In, T_GLVector3f v2In);

// 3D vector subtraction
void Vec3fSub(T_GLVector3f *vOut, T_GLVector3f v1In, T_GLVector3f v2In);

// Taubin's mesh smooth
void TaubinSmooth(GLfloat *vertices);

#endif	/* _R3DUTIL_H */

