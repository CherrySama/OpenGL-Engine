#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;



void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if(hdr)
    {
        // 1.reinhard更倾向明亮的区域，而暗的区域会不那么精细也不那么有区分度。
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));

        // 2.exposure曝光
        // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

        // 3.ACES (系数来自https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/)
        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;
        hdrColor *= exposure;
        vec3 result = (hdrColor * (a * hdrColor + b)) / (hdrColor * (c * hdrColor + d) + e);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}