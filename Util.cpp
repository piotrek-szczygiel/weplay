#include "Util.h"
#include <cstdlib>

float Random(float min, float max) {
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

int Random(int min, int max) {
	return rand() % (max - min) + min;
}

float Fade(float x, float y, float offset) {
	if (x > y) {
		x -= offset;
		if (x < y) {
			x = y;
		}
	} else if (x < y) {
		x += offset;
		if (x > y) {
			x = y;
		}
	}

	return x;
}