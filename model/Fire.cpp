#define _USE_MATH_DEFINES

#include "Fire.h"
#include <cmath> 
#include <math.h>
#include <vector>
#include <iostream>
#include "glm/vec3.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

Fire::Fire(float x, float y) : EBO(0), Object(x, y, 0) {  }

void Fire::generateVertices() {
    float fireVertices[] = {
       (-0.5f + 1.2f) * 0.25f, (0.0f - 1.6f) * 0.25f,  0.0f, 0.0f,  // Bottom-left
    (0.5f + 1.2f) * 0.25f, (0.0f - 1.6f) * 0.25f,  1.0f, 0.0f,  // Bottom-right
    (-0.3f + 1.2f) * 0.25f, (1.0f - 1.6f) * 0.25f,  0.0f, 1.0f,  // Top-left
    (0.3f + 1.2f) * 0.25f, (1.0f - 1.6f) * 0.25f,  1.0f, 1.0f   // Top-right
    };

    unsigned int fireIndices[] = {
        0, 1, 2,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fireVertices), fireVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fireIndices), fireIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
void Fire::draw(unsigned shaderProgram, float time) {
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);

    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

Fire::~Fire() {
    glDeleteTextures(1, &texture);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}