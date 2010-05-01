#include "DepthDecoder.h"

DepthDecoder::DepthDecoder() :
		width(0),
		height(0),
		mask(NULL),
		depth(NULL) {
}

DepthDecoder::~DepthDecoder() {
	if (mask != NULL) {
		delete [] mask;
		delete [] depth;
	}
}

int DepthDecoder::getWidth() {
	return width;
}

int DepthDecoder::getHeight() {
	return height;
}

bool* DepthDecoder::getMask() {
	return mask;
}

float* DepthDecoder::getDepth() {
	return depth;
}

void DepthDecoder::filterDeviations(float deviations) {
	int n = width * height;
	int total = 0;
	float mean = 0;
	for (int i = 0; i < n; i++) {
		if (!mask[i]) {
			mean += depth[i];
			total++;
		}
	}
	mean /= total;

	float sumSqDiff = 0;
	float diff;
	for (int i = 0; i < n; i++) {
		if (!mask[i]) {
			diff = mean - depth[i];
			sumSqDiff += diff * diff;
		}
	}

	float stdDev = sqrtf(sumSqDiff / total);
	float range = deviations * stdDev;

	filterRange(mean - range, mean + range);
}

/*
	Filtering the data is a destructive operation, since it
	overwrites the mask values. Sometimes we want to move the
	filtering min/max around a bit on a single decoded 3d frame.
	This is slow because the data has to be re-decoded each time.
	This could be avoided by toggling a flag that caches the
	original mask, and operates against that rather than in-place.
	It would be called something like setFastFilter(bool fastFilter).
*/
void DepthDecoder::filterRange(float min, float max) {
	int n = width * height;
	for (int i = 0; i < n; i++)
		if (!mask[i] && (depth[i] < min || depth[i] > max))
			mask[i] = true;
}

void DepthDecoder::exportDepth(string filename, float min, float max) {
	DepthExporter::exportDepth(filename, width, height, mask, depth, min, max);
}

void DepthDecoder::exportCloud(string filename) {
	DepthExporter::exportCloud(filename, width, height, mask, depth);
}

void DepthDecoder::exportMesh(string filename) {
	DepthExporter::exportMesh(filename, width, height, mask, depth);
}
