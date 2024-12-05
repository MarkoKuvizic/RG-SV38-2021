#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "stb_image.h"
#include "model/Island.h"
#include "model/Water.h"
#include "model/Fire.h"
#include "model/Circle.h"
#include "model/SharkFin.h"
#include "model/Square.h"
#include "model/TexturedObject.h"
#include "Global.hpp"
#include <random>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLFWvidmode* mode;
double mouseX = 0.0, mouseY = 0.0;
std::vector<SharkFin*> sharks;
std::vector<Circle*> stars;
bool circle;
Circle* indicator;
unsigned int bloodShader;

unsigned island_texture;
float duration;
float startTime; 

const float dayColor[3] = { 0.53f, 0.81f, 0.98f }; 
const float nightColor[3] = { 0.05f, 0.05f, 0.2f };
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "OpenGL Debug Message: " << message << std::endl;
}
GLFWcursor* createCursorFromImage(const char* filepath, int hotspotX, int hotspotY) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4); 
    if (!data) {
        printf("Failed to load cursor image\n");
        return NULL;
    }

    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = data;

    GLFWcursor* cursor = glfwCreateCursor(&image, hotspotX, hotspotY);
    stbi_image_free(data); 
    return cursor;
}


unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
unsigned int loadLetterTexture(const char* path);
bool renderSmokeLetters = false;
float smokeLetterCounter = 0.0;
std::vector<TexturedObject*> smokeLetters(0);

int main(void)
{

    std::vector<Object*> staticObjects(0);
    std::vector<unsigned int> letterTextures(0);
    float wAlpha = 1;
    
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    float t = 0;
    GLFWwindow* window;
    
    const char wTitle[] = "[SV-38/2021 Marko Kuvizic]";
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, wTitle, monitor, NULL);

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    std::vector<float> vertices;


    glfwMakeContextCurrent(window);

    GLFWcursor* customCursor = createCursorFromImage("res/cursor.png", 0, 0); 
    if (customCursor) {
        glfwSetCursor(window, customCursor);
    }

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int waterShader = createShader("water.vert", "water.frag");
    unsigned int moonShader = createShader("moon.vert", "basic.frag");
    unsigned int finShader = createShader("shark.vert", "basic.frag");
    unsigned int lightShader = createShader("basic.vert", "light.frag");
    unsigned int starShader = createShader("basic.vert", "star.frag");
    unsigned int fireShader = createShader("fire.vert", "fire.frag");
    unsigned int staticLightShader = createShader("basic.vert", "staticLight.frag");
    unsigned int cloudsShader = createShader("clouds.vert", "basic.frag");
    unsigned int smokeLettersShader = createShader("smokeLetter.vert", "smokeLetter.frag");
    unsigned int smokeShader = createShader("fire.vert", "smoke.frag");



   

    bloodShader = createShader("basic.vert", "blood.frag");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    unsigned int stride = (2 + 2) * sizeof(float);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    float extraSharkSpeed = 0;

    glUseProgram(unifiedShader);
    unsigned uTexLoc = glGetUniformLocation(unifiedShader, "uTex");
    glUniform1i(uTexLoc, 0);
    glUseProgram(0);

    

    //glDebugMessageCallback(DebugCallback, 0);
    //glEnable(GL_DEBUG_OUTPUT);

    island_texture = loadImageToTexture("res/sand.png");
    glBindTexture(GL_TEXTURE_2D, island_texture); //Podesavamo teksturu

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned tx = loadImageToTexture("res/water.png");
    glBindTexture(GL_TEXTURE_2D, tx); //Podesavamo teksturu

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    letterTextures.push_back(loadLetterTexture("res/alphabet/M.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/A.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/R.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/K.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/O.png"));

    letterTextures.push_back(loadLetterTexture("res/alphabet/U.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/V.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/I.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/Z.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/CH.png"));

    letterTextures.push_back(loadLetterTexture("res/alphabet/P.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/O.png"));



    letterTextures.push_back(loadLetterTexture("res/alphabet/S.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/V.png"));

    letterTextures.push_back(loadLetterTexture("res/alphabet/slash.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/3.png"));

    letterTextures.push_back(loadLetterTexture("res/alphabet/8.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/2.png"));

    letterTextures.push_back(loadLetterTexture("res/alphabet/0.png"));
    letterTextures.push_back(loadLetterTexture("res/alphabet/1.png"));

    unsigned palmTexture = loadLetterTexture("res/palm-tree.png");
    unsigned woodTexture = loadLetterTexture("res/wood-stack.png");
    unsigned c1Texture = loadLetterTexture("res/cloud1.png");
    unsigned c2Texture = loadLetterTexture("res/cloud2.png");
    unsigned c3Texture = loadLetterTexture("res/cloud3.png");


    unsigned sharkTexture = loadLetterTexture("res/shark.png");
    unsigned sunTexture = loadLetterTexture("res/sun.png");

    smokeLetters.push_back(new TexturedObject(0.3, -0.15, 0.2, 0.2, letterTextures[9]));
    smokeLetters.push_back(new TexturedObject(0.3, -0.15, 0.2, 0.2, letterTextures[4]));
    smokeLetters.push_back(new TexturedObject(0.3, -0.15, 0.2, 0.2, letterTextures[0]));
    smokeLetters.push_back(new TexturedObject(0.3, -0.15, 0.2, 0.2, letterTextures[4]));
    smokeLetters.push_back(new TexturedObject(0.3, -0.15, 0.2, 0.2, letterTextures[10]));



    for (auto s : smokeLetters) {
        s->generateVertices();
    }

    TexturedObject cloud1(-1, 0.4, 0.2, 0.4, c1Texture);
    cloud1.generateVertices();
    

    TexturedObject cloud2(-1, 0.35, 0.2, 0.4, c2Texture);
    cloud2.generateVertices();

    TexturedObject cloud3(-1, 0.35, 0.2, 0.4, c3Texture);
    cloud3.generateVertices();
    Island island1(0.4, 0.5, -0.6, island_texture);
    Island island3(0.3, -0.7, -0.6, island_texture);

    Circle moon(0.05, 0, 0, island_texture);
    Circle sun(0.1, 0, 0, sunTexture);

    float speed = 0.03;
    SharkFin sf(0, 0.7, sharkTexture, 1);
    SharkFin sf2(0, 0.7, sharkTexture, -1);


    sharks.push_back(&sf);
    sharks.push_back(&sf2);

    sf.generateVertices();
    sf2.generateVertices();
    sf2.rebuffer(1);
    Water water(0.1, tx);
   
    Fire fire(0.5, 0.5);
    fire.generateVertices();

    Fire smoke(0.5, 0.5);
    smoke.generateVertices();

    Fire smoke2(0.5, 0.5);
    smoke2.generateVertices();

    Fire smoke3(0.7, 0.7);
    smoke3.generateVertices();

    Fire fire2(0.5, 0.5);
    fire2.generateVertices();
    TexturedObject palm(0.6, 0, 1, 0.6, palmTexture);
    palm.generateVertices();

    TexturedObject wood(0.3, -0.4, 0.2, 0.2, woodTexture);
    wood.generateVertices();
    Square square(-1, -1, 2, 2, tx);
    Circle square2(1.2, 0.3, -0.4, tx);
    TexturedObject letterM(0.35, 0.95, 0.1, 0.1, letterTextures[0]);
    TexturedObject letterA(0.4, 0.95, 0.1, 0.1, letterTextures[1]);
    TexturedObject letterR(0.45, 0.95, 0.1, 0.1, letterTextures[2]);
    TexturedObject letterK(0.5, 0.95, 0.1, 0.1, letterTextures[3]);
    TexturedObject letterO(0.55, 0.95, 0.1, 0.1, letterTextures[4]);

    TexturedObject letterK2(0.65, 0.95, 0.1, 0.1, letterTextures[3]);
    TexturedObject letterU(0.7, 0.95, 0.1, 0.1, letterTextures[5]);
    TexturedObject letterV(0.75, 0.95, 0.1, 0.1, letterTextures[6]);
    TexturedObject letterI(0.8, 0.95, 0.1, 0.1, letterTextures[7]);
    TexturedObject letterZ(0.85, 0.95, 0.1, 0.1, letterTextures[8]);
    TexturedObject letterI2(0.9, 0.95, 0.1, 0.1, letterTextures[7]);
    TexturedObject letterCH(0.95, 0.95, 0.1, 0.1, letterTextures[9]);

    TexturedObject letterS(0.35, 0.85, 0.1, 0.1, letterTextures[12]);
    TexturedObject letterV3(0.4, 0.85, 0.1, 0.1, letterTextures[13]);
    TexturedObject letterSlash(0.45, 0.85, 0.1, 0.1, letterTextures[14]);

    TexturedObject letter3(0.50, 0.85, 0.1, 0.1, letterTextures[15]);
    TexturedObject letter8(0.55, 0.85, 0.1, 0.1, letterTextures[16]);
    TexturedObject letter2(0.63, 0.85, 0.1, 0.1, letterTextures[17]);
    TexturedObject letter0(0.68, 0.85, 0.1, 0.1, letterTextures[18]);
    TexturedObject letter22(0.73, 0.85, 0.1, 0.1, letterTextures[17]);
    TexturedObject letter1(0.78, 0.85, 0.1, 0.1, letterTextures[19]);



    std::vector<TexturedObject*> letters = std::vector<TexturedObject*>(0);

    letters.push_back(&letterM);
    letters.push_back(&letterA);
    letters.push_back(&letterR);
    letters.push_back(&letterK);
    letters.push_back(&letterO);
    letters.push_back(&letterK2);
    letters.push_back(&letterU);
    letters.push_back(&letterV);
    letters.push_back(&letterI);
    letters.push_back(&letterZ);
    letters.push_back(&letterI2);
    letters.push_back(&letterCH);

    letters.push_back(&letterS);
    letters.push_back(&letterV3);
    letters.push_back(&letter3);
    letters.push_back(&letter8);
    letters.push_back(&letterSlash);
    letters.push_back(&letter2);
    letters.push_back(&letter0);
    letters.push_back(&letter22);
    letters.push_back(&letter1);
    for (auto l : letters) {
        l->generateVertices();
    }

    water.generateVertices();
    square.generateVertices();
    square2.generateVertices();

    staticObjects.push_back(&island1);
    staticObjects.push_back(&island3);
    for (Object* o : staticObjects) {
        o->generateVertices();
    }
    moon.generateVertices();
    sun.generateVertices();

    std::random_device rd;

    std::uniform_real_distribution<float> dis(-1.0, 1.0);
    std::mt19937 gen(rd());
    glfwSwapInterval(1);
    for (int i = 0; i < 20; i++) {

        float x = dis(gen);
        float y = dis(gen);
        Circle* star = new Circle(0.003, x, y, 0);
        star -> generateVertices();
        stars.push_back(star);
    }

    while (!glfwWindowShouldClose(window))
    {
        float sTime = glfwGetTime();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        float alpha = 1.4f * sin((0.2f + t) / 10.0f);
        alpha = 1 - alpha;
        alpha = std::max(0.0f, std::min(1.0f, alpha));

        float clearColor[3];
        for (int i = 0; i < 3; ++i) {
            clearColor[i] = nightColor[i] * (1.0f - alpha) + dayColor[i] * alpha;
        }

        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        moon.draw(moonShader, t, 0);
        sun.draw(moonShader, t, 3.14);
        sf2.draw(finShader, speed, t);
        palm.draw(unifiedShader, t);

        glUseProgram(unifiedShader);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);     
        for (int i = 0; i < staticObjects.size(); i++) {
            
            staticObjects[i]->draw();

        }
        if (renderSmokeLetters) {
            float m = std::fmod((float)smokeLetters.size(), smokeLetterCounter);

            smokeLetters[std::floor(smokeLetterCounter)]->draw(smokeLettersShader, 1 - (smokeLetterCounter - std::floor(smokeLetterCounter)));
            smokeLetterCounter -= speed;
        }
        if (smokeLetterCounter < 0) {
            renderSmokeLetters = false;
        }
        for (auto star : stars) {
            star->draw(starShader, t, 0);
        }
        glBindVertexArray(0);
        glUseProgram(0);
        glUseProgram(waterShader);
        wood.draw(unifiedShader, t);

        sf.draw(finShader, speed, t);

        water.draw(waterShader, t, wAlpha);
        if (circle) {
            indicator->rebuffer();
            indicator->draw(bloodShader, glfwGetTime(), startTime, duration);
            extraSharkSpeed = 0.001;
            if (glfwGetTime() - startTime > duration){
                circle = false;
            }
        }
        else {
            int i = 0;

            for (SharkFin* s : sharks) {
                if (s->dontRebuffer) {
                    s->dontRebuffer = false;
                    s->target = -s->target;
                    if (abs(s->target) < 0.1) {
                        s->target = i % 2 * (-0.6);
                    }
                    i++;
                }
            }
            extraSharkSpeed = 0;
        }
        square.draw(lightShader, t);
        cloud1.draw(cloudsShader, std::fmod(t/10, 2.0));
        cloud2.draw(cloudsShader, std::fmod(t / 8, 2.0));
        cloud3.draw(cloudsShader, std::fmod(t / 11, 2.0));

        square2.draw(staticLightShader, t, 0);
        fire.draw(fireShader, glfwGetTime());
        fire2.draw(fireShader, t);
        smoke.draw(smokeShader, t + 0.1);
        smoke2.draw(smokeShader, t + 0.1);

        smoke3.draw(smokeShader, glfwGetTime() + 1);



        for (auto l : letters) {
            l->draw(unifiedShader, t);
        }

        
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        {
            
            speed += 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        {
            speed -= 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            if (wAlpha == 1)
                wAlpha = 0.4;
            else
                wAlpha = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            t = 0;
        }

        glfwSwapBuffers(window);

        t += speed;
        float timeTaken = sTime - glfwGetTime();
        if (timeTaken < 1.0f / 60.0f)
            glfwWaitEventsTimeout(1.0f / (60.0f - timeTaken));
        glfwPollEvents();

    }



    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);
    glDeleteProgram(waterShader);

    glfwTerminate();
    return 0;
}
unsigned int loadLetterTexture(const char* path) {
    unsigned int t = loadImageToTexture(path);
    glBindTexture(GL_TEXTURE_2D, t);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return t;
}
unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);
    
    if (glfwGetTime() - startTime > duration && circle) {
        circle = false;
    }
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &mouseX, &mouseY);
        float x = 2.0f * mouseX / mode->width - 1.0f;
        float y = 1.0f - 2.0f * mouseY / mode->height;
        std::cout << x << ", " << y << std::endl;
        if (y < -0.4) {
            duration = 3.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (5.0f - 3.0f)));
            startTime = glfwGetTime();
            circle = true;
            indicator = new Circle(0.1, x, y, island_texture);
            indicator->generateVertices();
            for (SharkFin* s : sharks) {
                s->target = 2.0f * mouseX / mode->width - 1.0f;
                s->dontRebuffer = true;
            }
        }
        else {
            if (x > 0.1 && x < 0.35 && y < -0.1 && y > -0.35) {
                renderSmokeLetters = true;
                smokeLetterCounter = (float)(smokeLetters.size() - 0.001);
            }
        }
    }
}