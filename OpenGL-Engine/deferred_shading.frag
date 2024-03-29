#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light{
	vec3 position;
	vec3 color;
	float linear;
	float quadratic;
	float radius;
};

const int NUM_LIGHTS = 32;
uniform Light lights[NUM_LIGHTS];
uniform vec3 viewPos;

void main()
{
	// retrieve data from g-buffer
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
	float Specular = texture(gAlbedoSpec, TexCoords).a;

	// then calculate light as usual
	vec3 lighting = Diffuse * 0.1; // 硬编码环境光照分量
	vec3 viewPos = normalize(viewPos - FragPos);
	for(int i = 0; i < NUM_LIGHTS; ++i)
	{
		float distance = length(lights[i].position - FragPos);
		if(distance < lights[i].radius)
		{
			// diffuse
			vec3 lightDir = normalize(lights[i].position - FragPos);
			vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].color;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewPos);
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
			vec3 specular = lights[i].color * spec * Specular;
			// attenuation
			float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);
		
			lighting += (diffuse + specular) * attenuation; 
		}
	}
 	FragColor = vec4(lighting, 1.0);
}
