#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;
uniform float alpha;
uniform sampler2D uTex; //teksturna jedinica

void main() {
    vec4 texColor = texture(uTex, chTex);
    
    texColor.a *= alpha; // Reduce alpha to 50% transparency
    
    outCol = texColor;
}

