#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projectorSpace;

void main() {
    gl_Position = projectorSpace * model * vec4(aPos, 1.0);
}
