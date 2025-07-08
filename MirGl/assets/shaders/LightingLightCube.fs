#version 330 core
out vec4 FragColor;
uniform mat4 model;       
uniform mat4 view;        
uniform mat4 projection;
void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}