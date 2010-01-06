#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef TWO_PI
#define TWO_PI (2*M_PI)
#endif

inline unsigned int log2(unsigned int value) {
	unsigned int l = 0;
	while( (value >> l) > 1 ) ++l;
	return l;
}
