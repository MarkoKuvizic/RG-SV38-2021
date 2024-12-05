#version 330 core

out vec4 FragColor;

uniform float time;     
uniform float startTime;
uniform float duration;  

void main() {
    float elapsed = time - startTime;
    if (elapsed > duration) {
        discard;
    }
    
    float alpha = 0.5 - (elapsed / duration)/2; 
    FragColor = vec4(vec3(1.0,0.0,0.0), alpha);
}