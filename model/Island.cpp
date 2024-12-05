#define _USE_MATH_DEFINES

#include "Island.h"
#include <cmath> 
#include <math.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Island::Island(float r, float x, float y, unsigned int texture) : radius(r), Object(x, y, texture) {}

void Island::generateVertices() {
  
    vertices.clear();

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.5f);  // Texture X
    vertices.push_back(0.5f);  // Texture Y

    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = (3.0f * M_PI * i) / SEGMENTS;  // Semi-circle angles [0, π]
        float vx = this->x + radius * cos(angle);
        float vy = this->y + radius * sin(angle);

        vertices.push_back(vx);
        vertices.push_back(vy);

        float s = 0.5f + 0.5f * cos(angle);
        float t = 0.5f + 0.5f * sin(angle);
        vertices.push_back(s);
        vertices.push_back(t);
    }

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Island::draw() {
    glActiveTexture(GL_TEXTURE0); 
    
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS + 2);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

}


Island::~Island() {
   glDeleteTextures(1, &texture);
   if (VBO) glDeleteBuffers(1, &VBO);
   if (VAO) glDeleteVertexArrays(1, &VAO);
}