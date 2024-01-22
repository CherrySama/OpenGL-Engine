#version 330 core
out vec4 FragColor;

struct SunLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float Kc;
	float Kl;
	float Kq;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_reflect1;
uniform samplerCube skybox;
uniform SunLight sunLight;
uniform PointLight pointLight;


vec3 useSunLight(SunLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 usePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    return ((ambient + diffuse + specular) * attenuation);
}

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 uNormal = texture(texture_normal1, TexCoords).rgb;
    uNormal = normalize(uNormal * 2.0f - 1.0f);
//    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, norm);

    vec3 reflection = texture(texture_reflect1, TexCoords).rgb;
    vec4 diffuse = texture(texture_diffuse1, TexCoords);

//    vec3 result = useSunLight(sunLight, uNormal, viewDir);
//    result += usePointLight(pointLight, norm, FragPos, viewDir);
//    FragColor = vec4(result, 1.0);
    FragColor = vec4(texture(skybox, R).rgb * reflection, 1.0) + diffuse;
}