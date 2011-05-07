/* 
 * File:   RCalibUtil.h
 * Author: jenicek
 *
 * Created on August 19, 2010, 12:18 PM
 */

#ifndef _RCALIBUTIL_H
#define	_RCALIBUTIL_H

#include "Global.h"
#include "RImageWindow.h"

typedef struct calibInput
{
    // Common
    int frameDelay;
    int numberBoards;

    // Camera parameters
    char *camStoragePath;
    int camResWidth;
    int camResHeight;
    float camGain;
    // Camera calibration chessboard parameters
    int camBoardWidth;
    int camBoardHeight;
    int camBoardSqMmW;
    int camBoardSqMmH;

    // Projector parameters
    char *projStoragePath;
    int projResWidth;
    int projResHeight;
    float projGain;
    // Projector calibration chessboard parameters
    int projBoardWidth;
    int projBoardHeight;
    int projBoardSqPxW;
    int projBoardSqPxH;
}T_CalibInput;

/*
 * Calibrated camera output parameters
*/
typedef struct calibOutput
{
    // Camera calibration output matrices
    CvMat* camIntrinsic;
    CvMat* camDistortion;
    CvMat* camExtrinsic;

    // Projector calibration output matrices
    CvMat* projIntrinsic;
    CvMat* projExtrinsic;
    CvMat* projDistortion;

    // Projector camera system geometry
    CvMat* camCenter;   // Camera center of projection
    CvMat* projCenter;  // Projector center of projection
}T_CalibOutput;

/*
 * Generate chessboard pattern for projector calibration
*/
int GenerateProjChboard(T_CalibInput* cInput, IplImage* chBoard, int* borderCols, int* borderRows);

/*
 * Calibrate camera function
*/
int CalibrateCamera(CvCapture *capture, RImageWindow *camWnd, T_CalibInput *cInput, T_CalibOutput *cOutput);

/*
 * Calibrate projector-camera system function
*/
int CalibrateCamProjSyst(CvCapture *capture, RImageWindow *camWnd, RImageWindow *projWnd, T_CalibInput *cInput, T_CalibOutput *cOutput);

/*
 * Computer projector-camera geometry
*/
int ComputePCGeometry(T_CalibInput *cInput, T_CalibOutput *cOutput);

#endif	/* _RCALIBUTIL_H */

