#include <vector>
#include "Object.h"
#pragma once

class Fire : public Object {
protected:
	unsigned int EBO;

public:
	Fire(float x, float y);

	void generateVertices();

	void draw(unsigned shader, float time);


	~Fire();
};
