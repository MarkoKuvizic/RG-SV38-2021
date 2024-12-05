#include <vector>
#include "Object.h"
#pragma once

class SharkFin : public Object {
public:
	float target;
	bool dontRebuffer = false;
	float startX = 0;
	float t = 0;
	SharkFin(float x, float y, unsigned texture, float target);
	SharkFin() {};

	void generateVertices();

	void draw(unsigned shaderProgram, float time, float realTime);
	void rebuffer(float t);

	~SharkFin();
};
