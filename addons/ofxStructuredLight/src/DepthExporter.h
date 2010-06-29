#pragma once

#ifdef OPENFRAMEWORKS_AVAIL
#include "ofMain.h"
#endif

#include <string>
#include <limits>
#include <fstream>

#include "libexport.h"

class DLL_EXPORT DepthExporter {
public:
	static void exportDepth(string filename, int width, int height, const bool* mask, const float* depth, float min, float max);
	static void exportCloud(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportTexture(string filename, int width, int height, const bool* mask, const unsigned char* color);
private:
	static std::string getExtension(std::string filename);

	static void exportObjVertex(std::ostream& obj, int x, int y, float z);
	static void exportObjCloud(std::string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportObjMesh(std::string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);

	static void exportPlyVertex(std::ostream& ply, float x, float y, float z, const unsigned char* color);
	static void exportPlyFace(std::ostream& ply, unsigned int a, unsigned int b, unsigned int c);
	static int exportPlyVertices(std::ostream& ply, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportPlyCloud(std::string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportPlyMesh(std::string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
};

