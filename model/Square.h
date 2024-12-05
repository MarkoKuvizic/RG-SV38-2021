#pragma once
#include <vector>
#include "Object.h"
#pragma once

class Square : public Object {
public:
	float target;
	bool dontRebuffer = false;
	float startX = 0;
	Square(float x, float y, float h, float w, unsigned texture);
	Square() {};

	void generateVertices();

	void draw(unsigned shaderProgram, float time);
	void rebuffer();

	~Square();
};
