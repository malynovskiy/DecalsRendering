#pragma once
#include <functional>

class TimeManager
{
  float deltaTime = 0.0f;// time between current frame and last frame
  float lastFrame = 0.0f;

  std::function<double(void)> getTimeProc;

  TimeManager(std::function<double(void)> timeProc) : getTimeProc{ timeProc } {}

public:
  static TimeManager &getInstance();

  static float getDeltaTime() { return getInstance().deltaTime; }

  void updateTime();
};