#version 330 core

out vec4 FragColor;
// in vec4 FragPosition;

uniform sampler2D depthMap;

float near = 0.1;
float far = 100.0;

void main() {
    // 1) Sample the depth buffer at the fragment's screen-space coordinates to get the depth of the geometry behind the decal.
    float depthMapValue = texelFetch(depthMap, ivec2(gl_FragCoord.xy), 0).r;
    float fragmentDepthValue = gl_FragCoord.z;
    
    float linearDepthMapValue = (2.0 * near) / (far + near - depthMapValue * (far - near));
    float linearFragmentDepthValue = (2.0 * near) / (far + near - fragmentDepthValue * (far - near));

    // 2) Compare the depth of the geometry with the depth of the decal fragment.
    if (linearDepthMapValue < linearFragmentDepthValue)
    {
        discard;
    }
    else
    {
        gl_FragDepth = depthMapValue;
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
}
