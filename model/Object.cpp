#include "Object.h"
#include <vector>
#include <iostream>
Object::Object(float x, float y, unsigned texture): x(x), y(y), vertices(*(new std::vector<float>(0))), VAO(0), VBO(0), texture(texture) {}
Object::Object() : vertices(*(new std::vector<float>(0))) {}

void Object::generateVertices() {};
void Object::draw() {};