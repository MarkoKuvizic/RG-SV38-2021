#include "Water.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Water::Water(float step, unsigned texture):step(step), EBO(0), indices(*(new std::vector<unsigned int>(0))), Object(0, -0.9, texture) {}

// Generate Vertices and Setup VAO/VBO
void Water::generateVertices() {
    vertices.clear();
    indices.clear();
    float width = 2;
    float height = 0.7;
    float gridWidth = 200;
    float gridHeight = 200;
    float dx = width / gridWidth;
    float dy = height / gridHeight;
    for (int i = 0; i <= gridWidth; ++i) {
        for (int j = 0; j <= gridHeight; ++j) {
            float xPos = x - width / 2 + i * dx;
            float yPos = y - height / 2 + j * dy;

            vertices.push_back(xPos);
            vertices.push_back(yPos);

            vertices.push_back((float)i / gridWidth);
            vertices.push_back((float)j / gridHeight);
        }
    }

    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            int topLeft = i * (gridHeight + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (gridHeight + 1) + j;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Water::draw(unsigned shaderProgram, unsigned time, float alpha) {
    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
    glUniform1f(glGetUniformLocation(shaderProgram, "alpha"), alpha);


    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}


Water::~Water() {
    glDeleteTextures(1, &texture);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (EBO) glDeleteBuffers(1, &EBO);

}