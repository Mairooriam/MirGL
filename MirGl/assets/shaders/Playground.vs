#version 330 core

layout(location = 0) in vec3 aPos;       // Vertex position
layout(location = 1) in vec3 aNormal;    // Vertex normal

out vec3 FragPos;                        // Pass the fragment position to the fragment shader
out vec3 Normal;                         // Pass the normal to the fragment shader
out vec3 FragColor;                      // Pass the object color to the fragment shader

uniform vec3 objectColor;                // Object color uniform
uniform mat4 model;                      // Model matrix
uniform mat4 view;                       // View matrix
uniform mat4 projection;                 // Projection matrix

void main() {
    // Transform the vertex position to clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Transform the vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform the normal to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass the object color to the fragment shader
    FragColor = objectColor;
}