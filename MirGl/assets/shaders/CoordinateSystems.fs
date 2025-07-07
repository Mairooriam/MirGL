#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform float fade;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    //FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);  
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), fade);
        // Debug output - show different colors in different regions
    // if (TexCoord.x < 0.5 && TexCoord.y < 0.5) {
    //     // Bottom-left: Show just texture1
    //     FragColor = texture(texture1, TexCoord);
    // }
    // else if (TexCoord.x >= 0.5 && TexCoord.y < 0.5) {
    //     // Bottom-right: Show just texture2
    //     FragColor = texture(texture2, TexCoord);
    // }
    // else if (TexCoord.x < 0.5 && TexCoord.y >= 0.5) {
    //     // Top-left: Show raw texture coordinates as colors
    //     FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);
    // }
    // else {
    //     // Top-right: Show the mixed result
    //     FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
    // }
}