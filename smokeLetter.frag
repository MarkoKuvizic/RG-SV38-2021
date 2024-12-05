#version 330 core

in vec2 chTex;
out vec4 outCol;
uniform float time;
uniform sampler2D uTex; 

void main() {
    vec4 texColor = texture(uTex, chTex);
    
    texColor.a *= (1-time); 
    
    outCol = texColor;
}

