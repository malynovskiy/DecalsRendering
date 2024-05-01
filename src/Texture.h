#pragma once

#include "Types.h"
#include <string>

struct Texture
{
  u32 descriptor;
  
  void bind() const;
  
  Texture& createTextureFromFile(std::string imagePath);
  static u32 loadTextureFromImage(char const *path);
};
