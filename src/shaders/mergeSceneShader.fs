#version 330 core

out vec4 FragColor;
in vec2 sceneUV;

uniform sampler2D renderedSceneTexture;

void main() {
    FragColor = texture(renderedSceneTexture, sceneUV);
}


// void main()
// {
// float far = 100.0f;
// float near = 0.1;

//   float depthValue = texture(renderedSceneTexture, sceneUV).r;
//   float linearDepthMapValue = (2.0 * near) / (far + near - depthValue * (far - near));
//   FragColor = vec4(vec3(linearDepthMapValue), 1.0);
// }
