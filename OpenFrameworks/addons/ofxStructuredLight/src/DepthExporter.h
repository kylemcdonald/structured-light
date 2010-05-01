#pragma once

#include "ofMain.h"
#include <limits>
#include <fstream>

class DepthExporter {
public:
	static void exportDepth(string filename, int width, int height, const bool* mask, const float* depth, float min, float max);
	static void exportCloud(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportTexture(string filename, int width, int height, const bool* mask, const unsigned char* color);
private:
	static string getExtension(string filename);

	static void exportObjVertex(ostream& obj, int x, int y, float z);
	static void exportObjCloud(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportObjMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);

	static void exportPlyVertex(ostream& ply, float x, float y, float z, const unsigned char* color);
	static void exportPlyFace(ostream& ply, unsigned int a, unsigned int b, unsigned int c);
	static int exportPlyVertices(ostream& ply, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportPlyCloud(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
	static void exportPlyMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color = NULL);
};
