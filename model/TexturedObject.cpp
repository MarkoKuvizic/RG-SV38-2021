#define _USE_MATH_DEFINES

#include "TexturedObject.h"
#include <cmath> 
#include <math.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

TexturedObject::TexturedObject(float x, float y, float h, float w, unsigned texture) : h(h), w(w), Object(x, y, texture) {}
void TexturedObject::generateVertices() {
    vertices.clear();

    float halfWidth = w / 2.0f;
    float halfHeight = h / 2.0f;

    this->vertices = {
        x - halfWidth, y - halfHeight, 0.0f, 0.0f,
        x + halfWidth, y - halfHeight, 1.0f, 0.0f,
        x + halfWidth, y + halfHeight, 1.0f, 1.0f, 

        x - halfWidth, y - halfHeight, 0.0f, 0.0f, 
        x + halfWidth, y + halfHeight, 1.0f, 1.0f, 
        x - halfWidth, y + halfHeight, 0.0f, 1.0f 
    };
    for (auto v : vertices) {
        std::cout << v << ", ";
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

}

void TexturedObject::draw(unsigned shaderProgram, float time) {
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);


    glActiveTexture(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

}


TexturedObject::~TexturedObject() {
    glDeleteTextures(1, &texture);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}