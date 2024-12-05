#include <vector>
#include "Object.h"
#pragma once

class Island: public Object {
protected:
	const int SEGMENTS = 50;
	float radius;
	

public:
	Island(float r, float x, float y, unsigned texture);

	void generateVertices();

	void draw();

	~Island();
};
