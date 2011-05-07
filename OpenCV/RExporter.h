/* 
 * File:   RExporter.h
 * Author: jenicek
 *
 * Created on August 23, 2010, 9:47 AM
 */

#ifndef _REXPORTER_H
#define	_REXPORTER_H

#include "Global.h"

int GetNumVertices(CvMat* mask);

bool ExportCloudToPly(char *fileName, CvMat* mask, CvMat* depth, CvMat *colors);

bool ExportMeshToPly(char *fileName, CvMat* mask, CvMat* depth, CvMat *colors);

#endif	/* _REXPORTER_H */

