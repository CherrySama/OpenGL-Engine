#version 330 core
out vec4 FragColor;

in vec3 TexCoords; // 代表3D纹理坐标的方向向量

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
    float gamma = 2.2f;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0f/gamma));
}