#pragma once

#include "Types.h"

#include <glm/vec4.hpp>

struct Framebuffer
{
  u32 id;
  u32 colorAttachment = 0;
  u32 depthAttachment = 0;

  void create(int width, int height, bool createColorAttachment = true, bool createDepthAttachment = true);

  void bind();
  void unbind();

  void clear(glm::vec4 color, bool clearColor = true, bool clearDepth = true);
};
