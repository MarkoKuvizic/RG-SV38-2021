#version 330 core

out vec4 FragColor;

uniform float time;       
void main() {
    float alpha = 1.4 * sin((0.2+time)/10);
    if (alpha > 0.7){
        alpha = 0.7;
    }
    FragColor = vec4(vec3(0,0,0), alpha);
}