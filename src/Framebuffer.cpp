#include "Framebuffer.h"

#include <glad/glad.h>

#include <iostream>

void Framebuffer::create(int width, int height, bool createColorAttachment, bool createDepthAttachment)
{
  // Generate framebuffer
  glGenFramebuffers(1, &id);
  glBindFramebuffer(GL_FRAMEBUFFER, id);

  // Create color attachment texture
  if (createColorAttachment)
  {
    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);
  }

  // Create depth attachment texture
  if (createDepthAttachment)
  {
    glGenTextures(1, &depthAttachment);
    glBindTexture(GL_TEXTURE_2D, depthAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
  }

  // Check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

  glBindTexture(GL_TEXTURE_2D, 0);
  unbind();
}

void Framebuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear(glm::vec4 color, bool clearColor /*= true*/, bool clearDepth /*= true*/)
{
  glClearColor(color.x, color.y, color.z, color.w);
  glClear((clearColor ? GL_COLOR_BUFFER_BIT : 0) | (clearDepth ? GL_DEPTH_BUFFER_BIT : 0));
}
