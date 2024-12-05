#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;
uniform float time;
uniform float theta;



void main()
{
	gl_Position = vec4(inPos.x + 1.4 * cos(theta + (0.1 * time)), inPos.y + 1 * sin(theta + (0.1 * time)), 0.0, 1.0);
	chTex = inTex;
}