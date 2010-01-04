#pragma once

class ofxImageSaver {
protected:
	unsigned int width, height, n;
	unsigned int position, total;
	vector<unsigned char*> images;
	vector<string> filenames;
public:
	ofxImageSaver() :
		width(0), height(0), n(0),
		position(0), total(0) {
	}
	~ofxImageSaver() {
		for(int i = 0; i < images.size(); i++)
			delete [] images[i];
	}
	void setup(unsigned int width, unsigned int height, unsigned int size) {
		this->width = width;
		this->height = height;
		n = width * height * 3;
		for(int i = 0; i < size; i++) {
			unsigned char* pixels = new unsigned char[n];
			images.push_back(pixels);
			filenames.push_back("");
		}
		cout << "Just allocated " << (float) (n * size) / (float) (1024 * 1024) << " MB for video." << endl;
	}
	void setFilename(string& filename) {
		filenames[position] = filename;
	}
	unsigned char* getPixels() {
		return images[position];
	}
	void next() {
		position++;
		position %= getMax();
		if(total != getMax())
			total++;
	}
	unsigned int getMax() {
		return images.size();
	}
	unsigned int getSize() {
		return total;
	}
	void saveAll() {
		cout << "Saving " << getSize() << " images... ";
		ofImage img;
		img.setUseTexture(false);
		img.allocate(width, height, OF_IMAGE_COLOR);
		for(int i = 0; i < getSize(); i++) {
			memcpy(img.getPixels(), images[i], n);
			img.saveImage(filenames[i]);
		}
		cout << "done." << endl;
	}
	void clear() {
		total = 0;
		position = 0;
	}
};
