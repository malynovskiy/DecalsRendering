#pragma once

struct GlobalInputState {
  float lastMousePositionX;
  float lastMousePositionY;
  bool firstMouse = true;
};

void processInput(GLFWwindow* window);