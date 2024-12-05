#include <vector>
#include "Object.h"
#pragma once

class Circle : public Object {
protected:
	const int SEGMENTS = 50;
	float radius;


public:
	Circle(float r, float x, float y, unsigned texture);

	void generateVertices();
	void rebuffer();

	void draw(unsigned shader, float time, float theta);
	void draw(unsigned shader, float time, float startTime, float duration);


	~Circle();
};
