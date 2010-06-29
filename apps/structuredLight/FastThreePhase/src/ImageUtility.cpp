#include "ImageUtility.h"

void ImageUtility::convertRgbToGrayscale(
		const unsigned char* input, unsigned char* output,
		int width, int height) {
	int n = width * height;
	int cur;
	for(int i = 0; i < n; i++) {
		cur = *(input++);
		cur += *(input++);
		cur += *(input++);
		cur /= 3;
		*(output++) = cur;
	}
}
