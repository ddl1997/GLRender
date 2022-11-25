#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform float downSampleSize;

uniform sampler2D equirectangularMap;

const float PI = 3.14159265359;
const vec2 invAtan = vec2(0.1591, 0.3183); // (1/2*pi, 1/pi)

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y)); // uv取值范围([-pi, pi], [-pi/2, pi/2])
    uv *= invAtan; // 转换到([-0.5, 0.5], [-0.5, 0.5])
    uv += 0.5; // 转换到([-1, 1], [-1, 1])
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(localPos));
    vec3 color = texture(equirectangularMap, uv).rgb;
    if (downSampleSize > 1.0)
    {
        vec3 irradiance = vec3(0.0);
        vec3 normal = normalize(localPos);
        vec3 up = vec3(0.0, 1.0, 0.0);
        vec3 right = cross(up, normal);
        up = cross(normal, right);

        float sampleDelta = 1.0 / downSampleSize;
        float nrSamples = 0.0;
        for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
        {
            for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
            {
                vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
                vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

                uv = SampleSphericalMap(normalize(sampleVec));
                irradiance += texture(equirectangularMap, uv).rgb * cos(theta) * sin(theta);
                nrSamples++;
            }
        }
        color = PI * irradiance * (1.0 / float(nrSamples));
    }

    FragColor = vec4(color, 1.0);
}