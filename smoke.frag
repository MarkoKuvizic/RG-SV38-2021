#version 330 core

in vec2 TexCoord;
in float yPos;

out vec4 FragColor;

uniform float time;

vec2 random2(vec2 p) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = dot(random2(i), f);
    float b = dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0));
    float c = dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0));
    float d = dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (mix(c, d, u.x) - mix(a, b, u.x)) * u.y;
}

vec3 fireGradient(float intensity) {
    vec3 baseColor = mix(vec3(0.1, 0.1, 0.0), vec3(1.0, 0.4, 0.0), intensity);  
    vec3 finalColor = mix(baseColor, vec3(0.0, 0.0, 0.0), 1.0 - intensity); 
    return finalColor;
}

void main() {
    float t = -time;

    float verticalGradient = 1.0 - smoothstep(0.0, 1.0, yPos);

    float flicker = noise(TexCoord * 5.0 + vec2(0.0, t * 4.0));
    float fireIntensity = verticalGradient * flicker;

    float distToCenter = length(TexCoord - vec2(0.5, 0.5));

    float edgeSoftness = smoothstep(0.4, 0.5, distToCenter); 

    fireIntensity *= (1.0 - edgeSoftness);  

    vec3 fireColor = fireGradient(fireIntensity);

  
    float emissionFactor = smoothstep(0.0, 0.4, distToCenter); 
    fireColor *= (1.0 + emissionFactor * 0.8);  

    
    FragColor = vec4(fireColor, fireIntensity);
}
