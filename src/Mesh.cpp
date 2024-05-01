#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

SimpleMesh createQuadMesh()
{
  const GLfloat quad_vertex_buffer_data[] = { -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,

    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f };
  const GLfloat quad_uv_data[] = { 0.0f,
    0.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,

    1.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f };

  SimpleMesh mesh{};
  glGenVertexArrays(1, &mesh.vao);
  glGenBuffers(1, &mesh.vbo);

  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer_data), quad_vertex_buffer_data, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &mesh.uv);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.uv);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_uv_data), quad_uv_data, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return mesh;
}

SimpleMesh createCubeMesh()
{
  SimpleMesh mesh{};
  glGenVertexArrays(1, &mesh.vao);
  glGenBuffers(1, &mesh.vbo);

  const GLfloat cube_vertex_buffer_data[] = { -1.0f,
    -1.0f,
    -1.0f,// triangle 1 : begin
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,// triangle 1 : end
    1.0f,
    1.0f,
    -1.0f,// triangle 2 : begin
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,// triangle 2 : end
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f };
  const GLfloat cube_uv_buffer_data[] = { 0.000059f,
    1.0f - 0.000004f,
    0.000103f,
    1.0f - 0.336048f,
    0.335973f,
    1.0f - 0.335903f,
    1.000023f,
    1.0f - 0.000013f,
    0.667979f,
    1.0f - 0.335851f,
    0.999958f,
    1.0f - 0.336064f,
    0.667979f,
    1.0f - 0.335851f,
    0.336024f,
    1.0f - 0.671877f,
    0.667969f,
    1.0f - 0.671889f,
    1.000023f,
    1.0f - 0.000013f,
    0.668104f,
    1.0f - 0.000013f,
    0.667979f,
    1.0f - 0.335851f,
    0.000059f,
    1.0f - 0.000004f,
    0.335973f,
    1.0f - 0.335903f,
    0.336098f,
    1.0f - 0.000071f,
    0.667979f,
    1.0f - 0.335851f,
    0.335973f,
    1.0f - 0.335903f,
    0.336024f,
    1.0f - 0.671877f,
    1.000004f,
    1.0f - 0.671847f,
    0.999958f,
    1.0f - 0.336064f,
    0.667979f,
    1.0f - 0.335851f,
    0.668104f,
    1.0f - 0.000013f,
    0.335973f,
    1.0f - 0.335903f,
    0.667979f,
    1.0f - 0.335851f,
    0.335973f,
    1.0f - 0.335903f,
    0.668104f,
    1.0f - 0.000013f,
    0.336098f,
    1.0f - 0.000071f,
    0.000103f,
    1.0f - 0.336048f,
    0.000004f,
    1.0f - 0.671870f,
    0.336024f,
    1.0f - 0.671877f,
    0.000103f,
    1.0f - 0.336048f,
    0.336024f,
    1.0f - 0.671877f,
    0.335973f,
    1.0f - 0.335903f,
    0.667969f,
    1.0f - 0.671889f,
    1.000004f,
    1.0f - 0.671847f,
    0.667979f,
    1.0f - 0.335851f };

  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW);
  //
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &mesh.uv);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.uv);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uv_buffer_data), cube_uv_buffer_data, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return mesh;
}

SimpleMesh createPrimitiveCube()
{
  SimpleMesh mesh{};
  glGenVertexArrays(1, &mesh.vao);
  glGenBuffers(1, &mesh.vbo);

  const GLfloat cube_vertex_buffer_data[] = { -1.0f,
    -1.0f,
    -1.0f,// triangle 1 : begin
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,// triangle 1 : end
    1.0f,
    1.0f,
    -1.0f,// triangle 2 : begin
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,// triangle 2 : end
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    1.0f };

  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW);
  //
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return mesh;
}

void deleteSimpleMesh(SimpleMesh mesh)
{
  glDeleteVertexArrays(1, &mesh.vao);
  glDeleteBuffers(1, &mesh.vbo);
  glDeleteBuffers(1, &mesh.uv);
}

void deletePrimitiveCube(SimpleMesh cube)
{
  glDeleteVertexArrays(1, &cube.vao);
  glDeleteBuffers(1, &cube.vbo);
}
