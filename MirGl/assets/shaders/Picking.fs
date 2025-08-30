#version 330 core

out uvec4 FragColor;

uniform uint objectID;

void main() {
    FragColor = uvec4(objectID, 0, 0, 1);  // Encode the object ID in the red channel
}