#include "TimeManager.h"

#include <GLFW/glfw3.h>

TimeManager &TimeManager::getInstance()
{
  static TimeManager instance{ glfwGetTime };
  return instance;
}

void TimeManager::updateTime()
{
  float currentFrame = static_cast<float>(getTimeProc());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}
