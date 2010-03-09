#include "StructuredLightDecoder.h"

class GrayDecoder : StructuredLightDecoder {
private:
	bool allocated;
	unsigned char threshold;
	bool* threshPixels;
	unsigned short *grayCoded, *binaryCoded, *binaryLUT;
	unsigned int curLevel, maxLevels, maxValue, size;

	void buildBinaryLUT() {
		for(unsigned short binary = 0; binary < (1 << maxLevels); binary++) {
			unsigned short gray = (binary >> 1) ^ binary;
			binaryLUT[gray] = binary;
		}
	}
	void buildGray() {
		unsigned short mask = maxValue;
		unsigned short on = 1 << (maxLevels - curLevel - 1);
		unsigned short off = (~on) & mask;
		for(int i = 0; i < size; i++)
			if(threshPixels[i])
				grayCoded[i] |= on;
			else
				grayCoded[i] &= off;
	}
	void buildBinary() {
		for(int i = 0; i < size; i++)
			binaryCoded[i] = binaryLUT[grayCoded[i]];
	}
	void getCodedDisplacement(ofImage& image, unsigned short* displacement) {
		unsigned char* pixels = image.getPixels();
		for(int i = 0; i < size; i++)
			pixels[i] = (255 * (int) displacement[i]) / maxValue;
		image.update();
	}
public:
	GrayDecoder() {
		allocated = false;
	}
	~GrayDecoder() {
		if(allocated) {
			delete [] threshPixels;
			delete [] grayCoded;
			delete [] binaryCoded;
			delete [] binaryLUT;
		}
	}
	void setup(int maxLevels, unsigned char threshold = 128) {
		this->maxLevels = maxLevels;
		this->threshold = threshold;
		maxValue = (1 << maxLevels) - 1;
	}
	void addImage(ofImage& image) {
		if(!allocated) {
			int width = image.width;
			int height = image.height;
			size = width * height;
			threshPixels = new bool[size];
			grayCoded = new unsigned short[size];
			binaryCoded = new unsigned short[size];
			binaryLUT = new unsigned short[1 << maxLevels];
			buildBinaryLUT();
			allocated = true;
		}

		const unsigned char* pixels = image.getPixels();
		for(int i = 0; i < size; i++)
			threshPixels[i] = pixels[i] > threshold;

		buildGray();
		buildBinary();

		curLevel = (curLevel + 1) % maxLevels;
	}
	int getCurLevel() {
		return curLevel;
	}
	void getThresholded(ofImage& image) {
		unsigned char* pixels = image.getPixels();
		for(int i = 0; i < size; i++)
			pixels[i] = threshPixels[i] ? 255 : 0;
		image.update();
	}
	void getGrayCodedDisplacement(ofImage& image) {
		getCodedDisplacement(image, grayCoded);
	}
	void getBinaryCodedDisplacement(ofImage& image) {
		getCodedDisplacement(image, binaryCoded);
	}
};
