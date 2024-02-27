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

// ��Ļ��ƽ����������������Ļ�ֱ��ʳ���������С��ֵ������
const vec2 noiseScale = vec2(1600.0/4.0, 900.0/4.0); // windowSize = (1600, 900)

void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
	// ����һ��TBN���󣬽����������߿ռ�任���۲�ռ�
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal)); // use Gram-Schmidt to create a Orthogonal Basis
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// ���������Ƕ�ÿ�������������е����������������߿ռ�任���۲�ռ䣬
	// �����Ǽӵ���ǰ����λ���ϣ�����Ƭ��λ������봢����ԭʼ��Ȼ����е�������Ƚ��бȽ�
	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; i++)
	{
		// get sample position
		vec3 samplePos = TBN * samples[i]; // TBN-space -> view-space
		samplePos = fragPos + samplePos * radius;
		// �任sample����Ļ�ռ䣬�Ӷ����ǾͿ�������ֱ����Ⱦ����λ�õ���Ļ��һ��ȡ��sample��(����)���ֵ
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset; // view-space -> clip-space
		offset.xyz /= offset.w; // perspective divide, clip-space -> ��׼���豸����(NDC)
		offset.xyz = offset.xyz * 0.5 + 0.5; // [-1,1] -> [0,1]
		// ʹ��offset������x��y�������������������Ӷ���ȡ����λ�ôӹ۲����ӽǵ����ֵ
		float sampleDepth = texture(gPosition, offset.xy).z;
		// ��������ĵ�ǰ���ֵ�Ƿ���ڴ洢�����ֵ
		// ����һ����Χ���ԴӶ���֤����ֻ���������ֵ��ȡ���뾶��ʱӰ���ڱ�����
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck; 
	}
	occlusion = 1.0 - (occlusion / kernelSize);

	FragColor = occlusion;
}