#version 330 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex;

uniform float time;
uniform float alpha;

out vec2 chTex;

void main() {
    float wave = sin(inPos.x * 5.0 + time) * 0.015; 
    gl_Position = vec4(inPos.x, inPos.y + wave + 0.1 * sin(4 * time * 2.0f * 3.14), 0.0, 1.0);

    chTex = inTex;
}
