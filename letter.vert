#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;
uniform float time;


void main() {

    chTex = inTex;

    
    gl_Position = vec4(inPos.x, inPos.y + time/10, 0.0, );

    
}
