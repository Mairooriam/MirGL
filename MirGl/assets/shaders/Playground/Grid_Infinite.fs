#version 330 core

out vec4 FragColor;

in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragView;
in mat4 fragProj;

uniform float gridSize = 1.0;
uniform vec3 gridColor = vec3(0.5, 0.5, 0.5);
uniform float lineWidth = 1.0;
uniform float fadeDistance = 0.1;
uniform float fadeStrength = 1.0;
uniform bool enableFading = true;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(gridColor, 1.0 - min(line, 1.0));
    
    // Draw axis lines in different colors
    if (drawAxis) {
        // Z axis (red line along X=0)
        if(fragPos3D.x > -lineWidth * minimumx && fragPos3D.x < lineWidth * minimumx)
            color.xyz = vec3(1.0, 0.0, 0.0);
        // X axis (green line along Z=0)
        if(fragPos3D.z > -lineWidth * minimumz && fragPos3D.z < lineWidth * minimumz)
            color.xyz = vec3(0.0, 1.0, 0.0);
    }
        
    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos, float near, float far) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D, 0.01, 100);
    float fading = max(0, (0.5 - linearDepth));

    // Draw grid with different scales for detail
    vec4 color1 = grid(fragPos3D, gridSize, true);     // Main grid with axes
    vec4 color2 = grid(fragPos3D, gridSize * 10, false); // Finer grid without axes
    
    // Blend the grids
    vec4 color = color1;
    color.a = max(color1.a, color2.a * 0.5);
    
    // Apply fading if enabled
    if (enableFading) {
        color.a *= fading * fadeStrength;
    }

    // Only draw if we're above the ground plane (t > 0)
    FragColor = color * float(t > 0);
}