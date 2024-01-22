#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;

// required when using a perspective projection matrix
float linearlizeDepth(float depth)
{
	float z = depth * 2.0f - 1.0f; // 深度值从[0,1]转换到[-1,1]范围的NDC
	return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
	float depthValue = texture(depthMap, TexCoords).r;
	// 这里正常应是depth = (linearlizeDepth(depthValue) - nearPlane) / (farPlane - nearPlane)
	// 但因为nearPlane的值较小，故直接省略
	// FragColor = vec4(vec3(linearlizeDepth(depthValue) / farPlane), 1.0f); // perspective
	FragColor = vec4(vec3(depthValue), 1.0f); // orthographic
}