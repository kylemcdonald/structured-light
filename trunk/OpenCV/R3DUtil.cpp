#include "R3DUtil.h"

T_GLVector2f Vec2fSet(GLfloat x, GLfloat y)
{
    T_GLVector2f vRes;
    vRes.x = x;
    vRes.y = y;
    return vRes;
}

T_GLVector3f Vec3fSet(GLfloat x, GLfloat y, GLfloat z)
{
    T_GLVector3f vRes;
    vRes.x = x;
    vRes.y = y;
    vRes.z = z;
    return vRes;
}

T_ColorRGBA ColorRGBASet(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    T_ColorRGBA colRes;
    colRes.r = r;
    colRes.g = g;
    colRes.b = b;
    colRes.a = a;
    return colRes;
}

float Vec3fDotP(T_GLVector3f v1, T_GLVector3f v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vec3fMag(T_GLVector3f v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Vec3fNormalize(T_GLVector3f *vec)
{
    float mag = Vec3fMag(*vec);

    if(mag != 0.0)
    {
        (*vec).x /= mag;
        (*vec).y /= mag;
        (*vec).z /= mag;
    }
}

void Vec3fCrossP(T_GLVector3f *vOut, T_GLVector3f v1In, T_GLVector3f v2In)
{
    T_GLVector3f vRes;
    vRes.x = v1In.y * v2In.z - v1In.z * v2In.y;
    vRes.y = v1In.z * v2In.x - v1In.x * v2In.z;
    vRes.z = v1In.x * v2In.y - v1In.y * v2In.x;

    (*vOut) = vRes;
}

void Vec3fMultScal(T_GLVector3f *vOut, T_GLVector3f vIn, GLfloat scalar)
{
    T_GLVector3f vRes;
    vRes.x = vIn.x * scalar;
    vRes.y = vIn.y * scalar;
    vRes.z = vIn.z * scalar;

    (*vOut) = vRes;
}

void Vec3fAdd(T_GLVector3f *vOut, T_GLVector3f v1In, T_GLVector3f v2In)
{
    T_GLVector3f vRes;
    vRes.x = v1In.x + v2In.x;
    vRes.y = v1In.y + v2In.y;
    vRes.z = v1In.z + v2In.z;

    (*vOut) = vRes;
}

void Vec3fSub(T_GLVector3f *vOut, T_GLVector3f v1In, T_GLVector3f v2In)
{
    T_GLVector3f vRes;
    vRes.x = v1In.x - v2In.x;
    vRes.y = v1In.y - v2In.y;
    vRes.z = v1In.z - v2In.z;

    (*vOut) = vRes;
}

void TaubinSmooth(GLfloat *vertices, unsigned int numVerts, unsigned int numIters)
{
    for(int n = 0; n < numIters; n++)
    {
        // Prepare array for the laplacean operators of the vertices
        CvMat* laOps = cvCreateMat(numVerts, numVerts, CV_32FC1);
        // Prepare array for weights of the lapl. operators
        CvMat* laWeights = cvCreateMat(1, numVerts, CV_32FC1);

        // Prepare storage for current two edge vertices
        CvPoint3D32f vert1 = cvPoint3D32f(0.0f, 0.0f, 0.0f);
        CvPoint3D32f vert2 = cvPoint3D32f(0.0f, 0.0f, 0.0f);

        
    }
}