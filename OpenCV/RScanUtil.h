/* 
 * File:   RScanUtil.h
 * Author: jenicek
 *
 * Created on July 29, 2010, 11:06 AM
 */

#ifndef _RSCANUTIL_H
#define	_RSCANUTIL_H

#include "Global.h"
#include "RPixelQueue.h"

#define PIX_UNWRAPPED 4000.0
#define PIX_BACKGROUND 4001.0

/*
 * Wrapping the phases
*/
void PhaseWrap(IplImage** phaseImages, CvMat** phase, CvMat** process, CvMat **iRatio, CvMat** colors, float cutoffThresh);
void PhaseWrap4(IplImage** phaseImages, CvMat** phase, CvMat** process, CvMat** iRatio, CvMat** colors, float cutoffThresh);
/*
 *
*/

/*
 * Simple Flood-Fill unwrapping
*/
CvPoint PhaseUnwrap(CvMat** phase, CvMat** process, CvMat** mask);
void UnwrapPixel(T_PixelQueue *pq, CvMat** phase, CvMat** process, CvMat **mask, float basePhase, int r, int c);
CvMat* FastBlur(CvMat* img, CvMat* process, int k);
CvPoint GetStart(CvMat *process);

/*
 * Quality guided Scan-Line phase unwrapping
*/
float WrapPhaseDifference(float ph1, float ph2);
void CreateQualityMap(CvMat *phase, CvMat* process, CvMat** quality);
void ComputeThresholds(CvMat *quality, float **thresholds, int numThresholds);
void PhaseUnwrapScanLine(CvMat **phase, CvMat **mask, CvMat **quality, float *thresholds, int numThresholds);
float UnwrapPix(float currPhase, float nbrPhase);
void UnwrapPatch(CvMat **phase, CvMat **quality, CvMat **phOffset, float threshold, CvPoint ptStart,
        int borderX, int borderY, int stepX, int stepY);

/*
 * Compensating phase errors using LUT
*/
void CreateLUT(double**lut, long lutSize);
void ApplyLUT(CvMat** phase, double *lut, long lutSize);
void ReleaseLUT(double **lut);

/*
 * Make the depth from the unwrapped phase map
*/
void MakeDepth(CvMat* phase, CvMat* mask, CvMat** depth, CvPoint startPt, float zskew, float zscale, bool bVertical);
void MakeDepth2(CvMat* phase, CvMat* mask, CvMat** depth, float f1, float s, float d);
void MakeDepth3(CvMat* phase, CvMat* mask, CvMat** depth, float theta, float d0, float l0, float fPitch);
void MakeDepth4(CvMat* phase, CvMat* mask, CvMat** depth);

/*
 * Line plane intersection in 3D
*/
void IntersectLinePlane3D(const float *q, const float *v, const float *w, float *p, float *depth);

/*
 * Line line intersection in 3D
*/
void IntersectLineLine3D(const float *q1, const float *v1, const float *q2, const float *v2, float *p);

/*
 * Find maximum of 3 floats
*/
float GetMax(float x, float y, float z);

/*
 * Find minimum of 3 floats
*/
float GetMin(float x, float y, float z);

/*
 * Get average brightness of a pixel
*/
float GetAvgBrightness(float red, float green, float blue);

/*
 * Get difference between two neighbour values
*/
float GetDiff(float val1, float val2);

/*void PhaseWrap4(IplImage** phaseImages, CvMat** phase, CvMat** process, CvMat** iRatio, CvMat** colors,
        float cutoffThresh)
 * Apply smooth on the phase map to remove noise
*/
void PhaseRemNoise(CvMat **phase, CvMat *mask);

/*
 * Save phase map to file
*/
int SavePhaseMap(char *fileName, CvMat *phase);

/*
 * Save color map to file 
*/
int SaveColorMap(char *fileName, CvMat *colors);

/*
 * Save mask
*/
int SaveMask(char *fileName, CvMat *mask);

/*
 * Change phase using ratio value
*/
void ChangePhaseFL(CvMat** phase, float ratio);

/*
 * Change image using ratio value
*/
void ChangeImg3CHUCH(CvMat** img, unsigned char ratio);

/*
 * Convert 3 channel float image to 3 channel uchar image
*/
void ImgFL3ToUCH3(CvMat* phaseFl, CvMat** phaseUch);

/*
 * Convert 1 channel float image to 3 channel uchar image
*/
void Img1FLTo3UCH(CvMat* phaseFl, CvMat** phaseUch);

/*
 * Convert 1 channel uchar image to 3 channel uchar image
*/
void Img1UCHTo3UCH(CvMat* phase1Uch, CvMat** phase3Uch);

#endif	/* _RSCANUTIL_H */

