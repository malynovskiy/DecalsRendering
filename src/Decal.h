#pragma once
#include "Transform.h"
#include "Texture.h"
#include "Mesh.h"

struct Decal
{
  Transform transform;
  SimpleMesh projectionBoxMesh;
  Texture texture;

  void setPosition(glm::vec3 newPosition);
  void setScale(glm::vec3 newScale);
};