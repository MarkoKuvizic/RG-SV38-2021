#version 330 core

out vec4 FragColor;

uniform float time;       

void main() {
    float cycleTime = mod(time/32, 2.0); 
    float alpha = 0.0;
    if (cycleTime < 0.8) {
        alpha = 0.5 + 0.5 * sin(time * 2.0); 
    } else {
        alpha = 0.0; 
    }
    FragColor = vec4(vec3(1.0,1.0,1.0), alpha);
}