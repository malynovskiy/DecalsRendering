#pragma once
#include "Types.h"

struct SimpleMesh
{
  u32 vbo, vao, uv;
};

SimpleMesh createQuadMesh();
SimpleMesh createCubeMesh();
SimpleMesh createPrimitiveCube();

void deleteSimpleMesh(SimpleMesh mesh);
void deletePrimitiveCube(SimpleMesh cube);
