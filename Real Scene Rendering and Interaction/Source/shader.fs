#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;
in vec4 outColor;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D OutTexture1;
uniform sampler2D OutTexture2;
uniform sampler2D OutTexture3;
uniform float tileSize;

void main()
{
    // texture
    vec2 tile = fract(TexCoord.xy / vec2(tileSize, tileSize));
    vec4 grass = texture(OutTexture1, tile);
    vec4 rock = texture(OutTexture2, tile);
    vec4 snow = texture(OutTexture3, tile);
    vec4 OutTexture = grass * outColor.x + rock * outColor.y + snow * outColor.z;

    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0) * OutTexture;
} 
