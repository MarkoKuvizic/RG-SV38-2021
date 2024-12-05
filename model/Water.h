#pragma once
#include "Object.h"
class Water : public Object {
protected:
	float step;
	unsigned EBO;
	std::vector<unsigned int> indices;

public:
	Water(float step, unsigned texture);

	void generateVertices();

	void draw(unsigned shaderProgram, unsigned time, float alpha);

	~Water();
};
