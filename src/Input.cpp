#include "Input.h"
#include "RenderingContext.h"

GlobalInputState globalInputState = { DEFAULT_WINDOW_WIDTH / 2.0f,DEFAULT_WINDOW_HEIGHT / 2.0f, true };

extern GlobalContext globalContext;

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (globalInputState.firstMouse)
  {
    globalInputState.lastMousePositionX = xpos;
    globalInputState.lastMousePositionY = ypos;
    globalInputState.firstMouse = false;
  }

  float xoffset = xpos - globalInputState.lastMousePositionX;
  float yoffset = globalInputState.lastMousePositionY - ypos; // reversed since y-coordinates go from bottom to top

  globalInputState.lastMousePositionX = xpos;
  globalInputState.lastMousePositionY = ypos;
  
  globalContext.camera->ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    globalContext.camera->ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    globalContext.camera->ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    globalContext.camera->ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    globalContext.camera->ProcessKeyboard(RIGHT, deltaTime);
}
