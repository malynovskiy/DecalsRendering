#include "Transform.h"

glm::mat4 Transform::getTransformMatrix()
{
  if (!transformMatrixValid)
  {
    const glm::mat4 Scale = glm::scale(Identity, scale);
    const glm::mat4 Translate = glm::translate(Identity, position);

    transformMatrix = Translate * Scale;
    transformMatrixValid = true;
  }

  return transformMatrix;
}

void Transform::setPosition(glm::vec3 newPosition)
{
  position = newPosition;
  transformMatrixValid = false;
}

void Transform::setScale(glm::vec3 newScale)
{
  scale = newScale;
  transformMatrixValid = false;
}

glm::mat4 Transform::constructTransformationMatrix(
  glm::vec3 translate, glm::vec3 scale, float angle, glm::vec3 rotationAxis)
{
  const glm::mat4 Rotate = glm::rotate(Identity, angle, rotationAxis);
  const glm::mat4 Scale = glm::scale(Identity, scale);
  const glm::mat4 Translate = glm::translate(Identity, translate);

  return Translate * Scale * Rotate;
}

// test function to regenerate objects transformations
void regenerateRandomTransformationMatrices(std::vector<glm::mat4> &tranformationMatrices)
{
  constexpr size_t cubesRows = 4;
  constexpr size_t cubesCols = 4;

  const size_t cubesNumber = cubesCols * cubesRows;

  if (tranformationMatrices.size() != cubesNumber)
    tranformationMatrices.resize(cubesNumber);

  constexpr float initialCubesSpacing = 1.5;
  constexpr float initialCubesOffset = 1.5;

  for (size_t i = 0; i < cubesRows; i++)
  {
    for (size_t j = 0; j < cubesCols; j++)
    {
      glm::mat4 &transformationMatrix = tranformationMatrices[i * cubesRows + j];

      glm::vec3 cubePosition = glm::vec3();
      cubePosition.x = -initialCubesOffset + j * initialCubesSpacing;
      cubePosition.y = initialCubesOffset - i * initialCubesSpacing;
      cubePosition.z = rand() % ((-4) - (-7) + 1) + (-7);

      const float cubeRotationAngle = rand() % (180 - 0 + 1) + 0;
      glm::vec3 cubeRotationAxis{};
      int randAxis = rand() % (2 - 0 + 1) + 0;
      if (randAxis == 0)
        cubeRotationAxis = glm::vec3(1, 0, 0);
      else if (randAxis == 1)
        cubeRotationAxis = glm::vec3(0, 1, 0);
      else
        cubeRotationAxis = glm::vec3(0, 0, 1);

      float randomScaleFactor = rand() % (8 - 3 + 1) + 3;
      glm::vec3 cubeScale = glm::vec3(0.1f * randomScaleFactor);


      transformationMatrix =
        Transform::constructTransformationMatrix(cubePosition, cubeScale, cubeRotationAngle, cubeRotationAxis);
    }
  }
}
