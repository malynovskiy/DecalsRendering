#pragma once

#include "Types.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgram
{
  u32 descriptor;

  ShaderProgram &create(std::string vertexShaderCode, std::string fragmentShaderCode);

  void bind() const;
  void destroy();

  void setUniform(const std::string &name, int value) const;
  void setUniform(const std::string &name, const glm::mat4& value) const;

  static u32 createShader(const char *shaderCode, u32 type);
};
