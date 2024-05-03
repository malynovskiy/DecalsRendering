#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 decalBoundingBoxUV;

uniform mat4 model;
uniform mat4 view_projection;
uniform mat4 decal_view_projection;

out vec4 FragPosition;

void main() {
    vec4 FragPosition = decal_view_projection * model * vec4(aPos, 1.0);
    gl_Position = view_projection * model * vec4(aPos, 1.0);
}
