#pragma once
#include <vector>
#include "Object.h"
#pragma once

class TexturedObject : public Object {
public:
	float h;
	float w;
	TexturedObject(float x, float y, float h, float w, unsigned texture);
	TexturedObject() {};

	void generateVertices();

	void draw(unsigned shaderProgram, float time);

	~TexturedObject();
};
