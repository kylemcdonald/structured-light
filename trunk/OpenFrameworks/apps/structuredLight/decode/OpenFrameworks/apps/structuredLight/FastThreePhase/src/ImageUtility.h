#pragma once

class ImageUtility {
public:
	static void convertRgbToGrayscale(
		const unsigned char* input, unsigned char* output,
		int width, int height);
};
