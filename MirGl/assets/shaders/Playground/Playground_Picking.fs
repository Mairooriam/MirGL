#version 330 core
out vec4 FragColor;

uniform int elementID;

void main()
{
    // Make colors brighter for debugging
    int id = elementID;
    
   // Option 1: Multiply by a large factor to make them visible
    float r = float((id & 0x000000FF)      ) / 255.0;
    float g = float((id & 0x0000FF00) >> 8 ) / 255.0;
    float b = float((id & 0x00FF0000) >> 16) / 255.0;

    FragColor = vec4(r, g, b, 1.0);

    // // Option 2: Simple bright colors based on ID
    // if (id == 1) FragColor = vec4(1.0, 0.0, 0.0, 1.0);      // Red
    // else if (id == 2) FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green  
    // else if (id == 3) FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue
    // else if (id == 4) FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Yellow
    // else FragColor = vec4(0.0, 0.0, 0.0, 1.0);              // Black for ID 0
}