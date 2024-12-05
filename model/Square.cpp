#define _USE_MATH_DEFINES

#include "Square.h"
#include <cmath> // for sin and cos
#include <math.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Number of segments for the semi-circle

Square::Square(float x, float y, float h, float w, unsigned texture) : Object(x, y, texture) {}
// Generate Vertices and Setup VAO/VBO
void Square::generateVertices() {
    // Clear any existing vertices
    vertices.clear();
    startX = x;

    // Add center vertex for the triangle fan
    for (int i = 0; i <= 12; ++i) {
        float angle = (4.0f * M_PI * i) / 12;  // Semi-circle angles [0, π]
        float vx = this->x + 4.0f * cos(angle);
        float vy = this->y + 4.0f * sin(angle);

        // Push vertex coordinates
        vertices.push_back(vx);
        vertices.push_back(vy);

        // Map texture coordinates
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
    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Draw the Island
void Square::draw(unsigned shaderProgram, float time) {
    glUseProgram(shaderProgram);

    
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

}


Square::~Square() {
    glDeleteTextures(1, &texture);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}