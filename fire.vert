#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord; 

out vec2 TexCoord;
out float yPos;

uniform float time;

vec2 smoothRandom(vec2 p) {
    float offsetX = sin(p.x * 3.0 + time * 0.5) * 0.05;
    float offsetY = cos(p.y * 3.0 + time * 0.5) * 0.05;
    return vec2(offsetX, offsetY);
}

void main() {
    
    vec2 offset = smoothRandom(aPos);

    vec2 newPos = aPos + offset;

    gl_Position = vec4(newPos, 0.0, 1.0);
    TexCoord = aTexCoord;
    yPos = newPos.y; 
}
