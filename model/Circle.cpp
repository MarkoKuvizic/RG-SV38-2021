#define _USE_MATH_DEFINES

#include "Circle.h"
#include <cmath> // for sin and cos
#include <math.h>
#include <vector>
#include <iostream>
#include "glm/vec3.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

Circle::Circle(float r, float x, float y, unsigned int texture) : radius(r), Object(x, y, texture) {  }

void Circle::generateVertices() {
    vertices.clear();
    std::cout << this->x << ", " << this->y << std::endl;
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.5f); 
    vertices.push_back(0.5f);  

    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = (4.0f * M_PI * i) / SEGMENTS; 
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
void Circle::rebuffer() {
    vertices.clear();
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.5f);  
    vertices.push_back(0.5f); 
    this->radius += 0.00001;
    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = (4.0f * M_PI * i) / SEGMENTS; 
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
void Circle::draw(unsigned shaderProgram, float time, float theta) {
    glUseProgram(shaderProgram);
    
    glActiveTexture(GL_TEXTURE0); // Activate texture unit 0

    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
    glUniform1f(glGetUniformLocation(shaderProgram, "theta"), theta);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS + 2);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

}
void Circle::draw(unsigned shaderProgram, float time, float startTime, float duration) {
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0); 

    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
    glUniform1f(glGetUniformLocation(shaderProgram, "startTime"), startTime);
    glUniform1f(glGetUniformLocation(shaderProgram, "duration"), duration);




    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS + 2);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

}


Circle::~Circle() {
    glDeleteTextures(1, &texture);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}