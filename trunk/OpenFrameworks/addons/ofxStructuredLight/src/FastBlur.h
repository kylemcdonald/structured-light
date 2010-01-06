#pragma once

class FastBlur {
public:
	FastBlur() {
		integral = NULL;
		sum = NULL;
	}
	void setup(int width, int height) {
		this->width = width;
		this->height = height;
		n = width * height;
		integral = new int[n];
		sum = new int[n];
	}
	~FastBlur() {
		delete [] sum;
		delete [] integral;
	}
	void blur(const unsigned char* img, const bool* mask, int k) {
		int i;
		for(i = 0; i < n; i += width)
			integral[i] = img[i]; // left side
		for(i = 0; i < width; i++)
			integral[i] = img[i]; // top side
		i = width; // start on second row
		for(int y = 1; y < height; y++) {
			i++; // skip left side
			for(int x = 1; x < width; x++) {
				integral[i] =
					img[i] + // current
					integral[i - 1] + // everything to the left
					integral[i - width] - // everything to the top
					integral[i - width - 1]; // the overlap
				i++; // next pixel
			}
		}
		memset(sum, 0, sizeof(int) * width * height);
		for(int y = k; y < height - k; y++) {
			for(int x = k; x < width - k; x++) {
				int i = y * width + x;
				if(mask[i]) {
					sum[i] =
						integral[i + k + k * width] + // bottom right
						integral[i - k - k * width] - // top left
						integral[i - k + k * width] - // bottom left
						integral[i + k - k * width]; // top right
				}
			}
		}
	}
	int* getSum() {
		return sum;
	}
protected:
	int width, height, n;
	int* integral;
	int* sum;
};
