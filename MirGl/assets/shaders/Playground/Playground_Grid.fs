#version 330 core

out vec4 FragColor;
in vec2 fragCoord; 

uniform float gridSize = 10.0;
uniform vec3 gridColor = vec3(0.5, 0.5, 0.5);
uniform float lineWidth = 0.02;
uniform float fadeDistance = 0.1;
uniform float fadeStrength = 1.0;
uniform bool enableFading = true;

void main() {
    vec2 grid = abs(fract(fragCoord / gridSize - 0.5) - 0.5) / fwidth(fragCoord / gridSize);
    float line = min(grid.x, grid.y);
    
    // Use lineWidth to control grid line thickness
    float alpha = 1.0 - smoothstep(0.0, lineWidth, line);
    
    // Configurable fading based on distance from center
    if (enableFading) {
        float fade = fadeStrength / (1.0 + length(fragCoord) * fadeDistance);
        alpha *= fade;
    }
    
    FragColor = vec4(gridColor, alpha);
}