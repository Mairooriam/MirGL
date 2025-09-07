#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
in vec3 FragPos;  
in vec3 Normal; 
in vec3 vertexColor;
void main()
{
    if (gl_PointCoord.x != 0.0 || gl_PointCoord.y != 0.0) {
        float dist = length(gl_PointCoord - vec2(0.5));
        if (dist > 0.5)
            discard;
    }


    float specularStrength = 0.5;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 baseColor = vertexColor * objectColor;
    vec3 result = (ambient + diffuse + specular) * baseColor;
    
    FragColor = vec4(result, 1.0);

}  