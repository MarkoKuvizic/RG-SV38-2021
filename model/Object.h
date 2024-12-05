#include <vector>

#pragma once
class Object {
public:
	float x;
	float y;
	unsigned int VAO;
	unsigned int VBO;
	std::vector<float>& vertices;
	unsigned texture;

	Object(float x, float y, unsigned texture);
	Object();
public:
	virtual void generateVertices();
	virtual void draw();
};