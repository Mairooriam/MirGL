#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;   // Available but not used
layout(location = 2) in vec3 aColor;    // Available but not used

out vec2 fragCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragCoord = aPos.xy; // Pass world coordinates to fragment shader
}