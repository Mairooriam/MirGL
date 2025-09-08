out vec4 FragColor;

uniform float gridSize = 1.0;
uniform vec3 gridColor = vec3(0.5, 0.5, 0.5);
uniform float lineWidth = 0.02;

void main() {
    vec2 grid = abs(fract(fragCoord / gridSize - 0.5) - 0.5) / fwidth(fragCoord / gridSize);
    float line = min(grid.x, grid.y);
    float alpha = 1.0 - min(line, 1.0);
    
    float fade = 1.0 / (1.0 + length(fragCoord) * 0.1);
    
    FragColor = vec4(gridColor, alpha * fade);
}