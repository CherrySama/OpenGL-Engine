#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//out VS_OUT {
//    vec2 texCoords;
//    vec3 fragPos;
//    vec3 normal;
//} vs_out;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
//    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;  
//    vs_out.texCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}