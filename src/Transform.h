#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

const glm::mat4 Identity = glm::mat4(1);

struct Transform
{
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;
  glm::mat4 transformMatrix;


  glm::mat4 getTransformMatrix();

  void setPosition(glm::vec3 newPosition);
  void setScale(glm::vec3 newScale);

  static glm::mat4 constructTransformationMatrix(
    glm::vec3 translate, glm::vec3 scale, float angle, glm::vec3 rotationAxis);

private:
  bool transformMatrixValid = false;
};

void regenerateRandomTransformationMatrices(std::vector<glm::mat4> &tranformationMatrices);
