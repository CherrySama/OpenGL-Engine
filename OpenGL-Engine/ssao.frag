#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform vec3 samples[64];
uniform mat4 projection;

// param
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// 屏幕的平铺噪声纹理会根据屏幕分辨率除以噪声大小的值来决定
const vec2 noiseScale = vec2(1600.0/4.0, 900.0/4.0); // windowSize = (1600, 900)

void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
	// 创建一个TBN矩阵，将向量从切线空间变换到观察空间
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal)); // use Gram-Schmidt to create a Orthogonal Basis
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// 接下来我们对每个核心样本进行迭代，将样本从切线空间变换到观察空间，
	// 将它们加到当前像素位置上，并将片段位置深度与储存在原始深度缓冲中的样本深度进行比较
	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; i++)
	{
		// get sample position
		vec3 samplePos = TBN * samples[i]; // TBN-space -> view-space
		samplePos = fragPos + samplePos * radius;
		// 变换sample到屏幕空间，从而我们就可像正在直接渲染它的位置到屏幕上一样取样sample的(线性)深度值
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset; // view-space -> clip-space
		offset.xyz /= offset.w; // perspective divide, clip-space -> 标准化设备坐标(NDC)
		offset.xyz = offset.xyz * 0.5 + 0.5; // [-1,1] -> [0,1]
		// 使用offset向量的x和y分量采样线性深度纹理从而获取样本位置从观察者视角的深度值
		float sampleDepth = texture(gPosition, offset.xy).z;
		// 检查样本的当前深度值是否大于存储的深度值
		// 引入一个范围测试从而保证我们只当被测深度值在取样半径内时影响遮蔽因子
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck; 
	}
	occlusion = 1.0 - (occlusion / kernelSize);

	FragColor = occlusion;
}