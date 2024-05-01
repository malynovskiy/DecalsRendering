#pragma once

#include <string>

constexpr size_t DEFAULT_WINDOW_WIDTH = 2560;
constexpr size_t DEFAULT_WINDOW_HEIGHT = 1440;

struct GLFWwindow;
class Camera;

struct GlobalContext {
  Camera* camera;
  GLFWwindow* window;
  GLFWwindow* secondaryWindow;
  int windowWidth;
  int windowHeight;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool createRenderingContext();
