#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;

uniform float directionChange;
uniform float movementDirection;
uniform float time;

void main()
{
	float wave = sin(inPos.x * 5.0 + time) * 0.015; 
	gl_Position = vec4(inPos.x + movementDirection, -1.3 + wave + inPos.y + 0.1 * sin((4 * time/1750 * 2.0f * 3.14) + 0.75*3.14), 0.0, 1.0);
	chTex = inTex;

}