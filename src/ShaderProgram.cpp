#include "ShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static constexpr u32 InfoBufferSize = 512;

ShaderProgram &ShaderProgram::create(std::string vertexShaderCode, std::string fragmentShaderCode)
{
  GLuint vertexShader = createShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
  GLuint fragmentShader = createShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint success{};
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[InfoBufferSize];
    glGetShaderInfoLog(shaderProgram, InfoBufferSize, nullptr, infoLog);

    std::string output{};
    output += "Failed linkage of GLSL shaders into a shader program.\n";
    output += "Program info log:\n" + std::string(infoLog) + '\n';
    assert(false);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  descriptor = shaderProgram;
  return *this;
}

void ShaderProgram::use() const
{
  glUseProgram(descriptor);
}

void ShaderProgram::destroy()
{
    glDeleteProgram(descriptor);
}

void ShaderProgram::setUniform(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(descriptor, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4& value) const
{
  glUniformMatrix4fv(glGetUniformLocation(descriptor, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


GLuint ShaderProgram::createShader(const char *shaderCode, u32 type)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &shaderCode, nullptr);
  glCompileShader(shader);

  GLint success{};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[InfoBufferSize];
    glGetShaderInfoLog(shader, InfoBufferSize, nullptr, infoLog);

    const std::string shaderTypeStr = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
    std::string output = "";
    output += "GLSL compile error" + shaderTypeStr + " shader: '" + shaderCode + "'\n\n";
    output += "Shader info log:\n" + std::string(infoLog) + '\n';
    printf("%s", output.data());
    assert(false);
    return -1;
  }

  return shader;
}