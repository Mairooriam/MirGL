#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform mat4 model;       
uniform mat4 view;        
uniform mat4 projection;
void main()
{
    FragColor = vec4(objectColor, 1.0);
}