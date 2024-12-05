#define _USE_MATH_DEFINES

#include "SharkFin.h"
#include <cmath> 
#include <math.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>


SharkFin::SharkFin(float x, float y, unsigned texture, float target) : target(target), startX(x), Object(x, y, texture) {}
void SharkFin::generateVertices() {
    vertices.clear();

    float halfWidth = 0.5 / 2.0f;
    float halfHeight = 0.5 / 2.0f;

    this->vertices = {
        x - halfWidth, y - halfHeight, 0.0f, 0.0f, // dl
        x + halfWidth, y - halfHeight, 1.0f, 0.0f, // dd
        x + halfWidth, y + halfHeight, 1.0f, 1.0f, // gd

        x - halfWidth, y - halfHeight, 0.0f, 0.0f, // dl2
        x + halfWidth, y + halfHeight, 1.0f, 1.0f, // gd2
        x - halfWidth, y + halfHeight, 0.0f, 1.0f  // gl
    };
    std::vector<float> v(0);
    for (auto vv : vertices) {
        v.push_back(vv);
    }
    vertices[0] = v[4];
    vertices[4] = v[0];
    vertices[8] = v[12];
    vertices[12] = v[8];
    vertices[16] = v[20];
    vertices[20] = v[16];

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
void SharkFin::rebuffer(float t) {
    std::vector<float> v(0);
    for (auto vv : vertices) {
        v.push_back(vv);
    }
    vertices[0] = v[4];
    vertices[4] = v[0];
    vertices[8] = v[12];
    vertices[12] = v[8];
    vertices[16] = v[20];
    vertices[20] = v[16];

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

void SharkFin::draw(unsigned shaderProgram, float time, float realTime) {
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), realTime);
    float prevT = t;
    float q = 5;
    if (dontRebuffer) {
        q = 2;
    }
    if (startX > target)
        t = -time/q;
    else
        t = time/q;
    if (prevT * t < 0) {
        glUniform1f(glGetUniformLocation(shaderProgram, "directionChange"), 1);
        rebuffer(t);
    }
    if (abs(startX - target) < 0.01 && !dontRebuffer) {
        std::random_device rd;
        std::mt19937 gen(rd()); 

        std::uniform_real_distribution<> dist(-1.0, 1.0);

        target = dist(gen);
    }
    startX += t;
    
    glUniform1f(glGetUniformLocation(shaderProgram, "movementDirection"), startX);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);


    glActiveTexture(GL_TEXTURE0); 

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

}


SharkFin::~SharkFin() {
    glDeleteTextures(1, &texture);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}