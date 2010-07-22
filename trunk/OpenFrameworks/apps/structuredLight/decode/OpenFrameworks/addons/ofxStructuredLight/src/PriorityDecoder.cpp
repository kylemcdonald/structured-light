#include "PriorityDecoder.h"

PriorityDecoder::PriorityDecoder() :
	distance(NULL) {
}

PriorityDecoder::~PriorityDecoder() {
	if(distance != NULL)
		delete [] distance;
}

float PriorityDecoder::diff(float a, float b) {
  float d = a < b ? b - a : a - b;
  return d < .5 ? d : 1 - d;
}

void PriorityDecoder::unwrapPhase() {
	if(distance == NULL)
		distance = new float[width * height];

  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
    	int i = y * width + x;
      if(!mask[i]) {
        distance[i] = (
          diff(phase[i], phase[i - 1]) +
          diff(phase[i], phase[i + 1]) +
          diff(phase[i], phase[i - width]) +
          diff(phase[i], phase[i + width])) / range[i];
      }
    }
  }

	minPhase = 1e6;
	maxPhase = -1e6;

	startInd = getStart();
	if (phasePersistence) {
		unwrapPhase(startInd, 0, lastPhase[startInd]);
	} else {
		unwrapPhase(startInd, 0, phase[startInd]);
	}

	int ind = 0;

	while (!toProcess.empty()) {
		const WrappedPixel& cur = toProcess.top();
		int i = cur.target;
		if (ready[i]) {
			phase[i] = cur.phase;
			ready[i] = false;

			unwrapOrder[i] = ind++;
			if (phase[i] > maxPhase) maxPhase = phase[i];
			if (phase[i] < minPhase) minPhase = phase[i];

			float d = cur.distance;
			float r = cur.phase;

			int x = i % width;
			int y = i / width;

			if (y > 0) // up
				unwrapPhase(i - width, d, r);
			if (y < height - 1) // down
				unwrapPhase(i + width, d, r);
			if (x > 0) // left
				unwrapPhase(i - 1, d, r);
			if (x < width - 1) // right
				unwrapPhase(i + 1, d, r);
		} else {
			toProcess.pop();
		}
	}
}

inline void PriorityDecoder::unwrapPhase(int target, float d, float r) {
	if (ready[target]) {
		float diff = phase[target] - (r - (int) r);
		if (diff > .5)
			diff--;
		else if (diff < -.5)
			diff++;
		toProcess.push(WrappedPixel(target, d + distance[target], r + diff));
	}
}
