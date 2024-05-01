#version 330 core

out vec4 FragColor;
in vec2 uv;

uniform sampler2D texture1;

void main() {
    FragColor = vec4(texture(texture1, uv).rgb, 1.0f);
}
