#version 330 core

in vec2 TexCoord;  
out vec4 FragColor;  

uniform float time; 

float smoothNoise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    float dist = length(TexCoord - vec2(0.5, 0.5)); 

    float sineFade = max(0.0, 1.4 * sin((0.2 + time) / 10.0));

    float distFade = smoothstep(0.0, 0.8, 1.0 - (dist * 6.0));  

    float flicker = 0.9 + 0.1 * smoothNoise(vec2(time * 1.2, time * 1.8));  

    float alpha = 0.04 * sineFade * distFade * flicker; 

    FragColor = vec4(vec3(1.0, 0.5, 0.2), alpha); 
}
