#include "DepthExporter.h"

/*
 More information about OBJ and PLY are available at:
 http://local.wasp.uwa.edu.au/~pbourke/dataformats/obj/
 http://local.wasp.uwa.edu.au/~pbourke/dataformats/ply/
*/

void DepthExporter::exportDepth(string filename, int width, int height, const bool* mask, const float* depth, float min, float max) {
	ofImage img;
	img.allocate(width, height, OF_IMAGE_GRAYSCALE);
	unsigned char* pixels = img.getPixels();
	int n = width * height;
	for (int i = 0; i < n; i++) {
		if (mask[i]) {
			pixels[i] = 0;
		} else {
			pixels[i] = (unsigned char) ofClamp(ofMap(depth[i], min, max, 1, 256), 1, 255);
		}
	}

	img.saveImage(filename);
}

string DepthExporter::getExtension(string filename) {
	int i = filename.rfind('.');
	if (i != string::npos) {
		return filename.substr(i + 1);
	} else {
		return "";
	}
}

void DepthExporter::exportCloud(string filename, int width, int height, const bool* mask, const float* depth,  const unsigned char* color) {
	string extension = getExtension(filename);
	if (extension == "obj") {
		exportObjCloud(filename, width, height, mask, depth, color);
	} else if (extension == "ply") {
		exportPlyCloud(filename, width, height, mask, depth, color);
	}
}

void DepthExporter::exportMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color) {
	string extension = getExtension(filename);
	if (extension == "obj") {
		exportObjMesh(filename, width, height, mask, depth, color);
	} else if (extension == "ply") {
		exportPlyMesh(filename, width, height, mask, depth, color);
	}
}

void DepthExporter::exportTexture(string filename, int width, int height, const bool* mask, const unsigned char* color) {
	ofImage img;
	img.allocate(width, height, OF_IMAGE_COLOR);
	unsigned char* pixels = img.getPixels();
	int n = width * height;

	for (int i = 0; i < n; i++) {
		if (mask[i]) {
			pixels[i * 3 + 0] = 0;
			pixels[i * 3 + 1] = 0;
			pixels[i * 3 + 2] = 0;
		} else {
			pixels[i * 3 + 0] = color[i * 3 + 0];
			pixels[i * 3 + 1] = color[i * 3 + 1];
			pixels[i * 3 + 2] = color[i * 3 + 2];
		}
	}

	img.saveImage(filename);
}

// Flipping the y is one way to orient the model correctly.
inline void DepthExporter::exportObjVertex(ostream& obj, int x, int y, float z) {
	obj << "v " << x << " " << -y << " " << z << endl;
}

void DepthExporter::exportObjCloud(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color) {
	ofstream obj;
	obj.open(ofToDataPath(filename).c_str(), ios::out);
	if (obj.is_open()) {
		int total = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int i = y * width + x;
				if (!mask[i]) {
					exportObjVertex(obj, x, y, depth[i]);
					total++;
				}
			}
		}
		obj << "p";
		for (int i = 1; i <= total; i++)
			obj << " " << i;
		obj << endl;
	}
}

void DepthExporter::exportObjMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color) {
	ofstream obj;
	obj.open(ofToDataPath(filename).c_str(), ios::out);
	if (obj.is_open()) {
		int total = 0;
		int nw, ne, sw, se;
		for (int y = 0; y < height - 1; y++) {
			for (int x = 0; x < width - 1; x++) {
				nw = y * width + x;
				ne = nw + 1;
				sw = nw + width;
				se = ne + width;
				if (!mask[nw] && !mask[se]) {
					if (!mask[ne]) {
						exportObjVertex(obj, x + 1, y + 1, depth[se]);
						exportObjVertex(obj, x + 1, y, depth[ne]);
						exportObjVertex(obj, x, y, depth[nw]);
						obj << "f -3 -2 -1" << endl;
					}
					if (!mask[sw]) {
						exportObjVertex(obj, x, y + 1, depth[sw]);
						exportObjVertex(obj, x + 1, y + 1, depth[se]);
						exportObjVertex(obj, x, y, depth[nw]);
						obj << "f -3 -2 -1" << endl;
					}
				} else if (!mask[ne] && !mask[sw]) {
					if (!mask[nw]) {
						exportObjVertex(obj, x, y + 1, depth[sw]);
						exportObjVertex(obj, x + 1, y, depth[ne]);
						exportObjVertex(obj, x, y, depth[nw]);
						obj << "f -3 -2 -1" << endl;
					}
					if (!mask[se]) {
						exportObjVertex(obj, x, y + 1, depth[sw]);
						exportObjVertex(obj, x + 1, y + 1, depth[se]);
						exportObjVertex(obj, x + 1, y, depth[ne]);
						obj << "f -3 -2 -1" << endl;
					}
				}
			}
		}
	}
}

// Flipping the y is one way to orient the model correctly.
inline void DepthExporter::exportPlyVertex(ostream& ply, float x, float y, float z, const unsigned char* color) {
	y = -y;
	ply.write(reinterpret_cast<char*>(&x), sizeof(float));
	ply.write(reinterpret_cast<char*>(&y), sizeof(float));
	ply.write(reinterpret_cast<char*>(&z), sizeof(float));
	if (color != NULL)
		ply.write((char*) color, sizeof(char) * 3);
}

inline void DepthExporter::exportPlyFace(ostream& ply, unsigned int a, unsigned int b, unsigned int c) {
	ply << (unsigned char) 3;
	ply.write(reinterpret_cast<char*>(&a), sizeof(unsigned int));
	ply.write(reinterpret_cast<char*>(&b), sizeof(unsigned int));
	ply.write(reinterpret_cast<char*>(&c), sizeof(unsigned int));
}

int DepthExporter::exportPlyVertices(ostream& ply, int width, int height, const bool* mask, const float* depth, const unsigned char* color) {
	int total = 0;
	for (int y = 0; y < height; y++) {
		int i = y * width;
		for (int x = 0; x < width; x++) {
			if (!mask[i]) {
				exportPlyVertex(ply, x, y, depth[i], color == NULL ? NULL : &color[i * 3]);
				total++;
			}
			i++;
		}
	}
	return total;
}

void DepthExporter::exportPlyCloud(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color) {
	ofstream ply;
	ply.open(ofToDataPath(filename).c_str(), ios::out | ios::binary);
	if (ply.is_open()) {
		// create all the vertices
		stringstream vertices(ios::in | ios::out | ios::binary);
		int total = exportPlyVertices(vertices, width, height, mask, depth, color);

		// write the header
		ply << "ply" << endl;
		ply << "format binary_little_endian 1.0" << endl;
		ply << "element vertex " << total << endl;
		ply << "property float x" << endl;
		ply << "property float y" << endl;
		ply << "property float z" << endl;
		if (color != NULL) {
			ply << "property uchar red" << endl;
			ply << "property uchar green" << endl;
			ply << "property uchar blue" << endl;
		}
		ply << "end_header" << endl;

		// write all the vertices
		ply << vertices.rdbuf();
	}
}

void DepthExporter::exportPlyMesh(string filename, int width, int height, const bool* mask, const float* depth, const unsigned char* color) {
	ofstream ply;
	ply.open(ofToDataPath(filename).c_str(), ios::out | ios::binary);
	if (ply.is_open()) {
		int n = width * height;
		unsigned int* names = new unsigned int[n];

		// label all the vertices
		int total = 0;
		for (int i = 0; i < n; i++)
			if (!mask[i])
				names[i] = total++;

		// create all the vertices
		stringstream vertices(ios::in | ios::out | ios::binary);
		exportPlyVertices(vertices, width, height, mask, depth, color);

		// create all the faces
		int totalFaces = 0;
		int nw, ne, sw, se;
		stringstream faces(ios::in | ios::out | ios::binary);
		for (int y = 0; y < height - 1; y++) {
			for (int x = 0; x < width - 1; x++) {
				nw = y * width + x;
				ne = nw + 1;
				sw = nw + width;
				se = ne + width;
				if (!mask[nw] && !mask[se]) {
					if (!mask[ne]) {
						exportPlyFace(faces, names[se], names[ne], names[nw]);
						totalFaces++;
					}
					if (!mask[sw]) {
						exportPlyFace(faces, names[sw], names[se], names[nw]);
						totalFaces++;
					}
				} else if (!mask[ne] && !mask[sw]) {
					if (!mask[nw]) {
						exportPlyFace(faces, names[sw], names[ne], names[nw]);
						totalFaces++;
					}
					if (!mask[se]) {
						exportPlyFace(faces, names[sw], names[se], names[ne]);
						totalFaces++;
					}
				}
			}
		}

		// write the header
		ply << "ply" << endl;
		ply << "format binary_little_endian 1.0" << endl;
		ply << "element vertex " << total << endl;
		ply << "property float x" << endl;
		ply << "property float y" << endl;
		ply << "property float z" << endl;
		if (color != NULL) {
			ply << "property uchar red" << endl;
			ply << "property uchar green" << endl;
			ply << "property uchar blue" << endl;
		}
		ply << "element face " << totalFaces << endl;
		ply << "property list uchar uint vertex_indices" << endl;
		ply << "end_header" << endl;

		ply << vertices.rdbuf();
		ply << faces.rdbuf();

		delete [] names;
	}
}
