#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTex;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{    
    vec3 color = texture(floorTex, fs_in.TexCoords).rgb;    
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    // 
    float distances = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (blinn ? distances : distances * distances); 
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    // assuming bright white light color
    vec3 specular = vec3(0.3) * spec; 
    FragColor = vec4((ambient + diffuse + specular)*attenuation, 1.0);
    float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0f/gamma));
//    FragColor = texture(floorTex, fs_in.TexCoords);
}