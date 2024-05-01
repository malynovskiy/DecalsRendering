#include "Types.h"
#include "Utils.h"

#include "TimeManager.h"
#include "Camera.h"

#include "RenderingContext.h"
#include "ShaderProgram.h"
#include "Decal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

void processInput(GLFWwindow *window);

extern GlobalContext globalContext;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = DEFAULT_WINDOW_WIDTH / 2.0f;
float lastY = DEFAULT_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

int main()
{
  if (!createRenderingContext() || globalContext.window == nullptr)
  {
    return -1;
  }

  globalContext.camera = &camera;

  static_assert(sizeof(GLuint) == sizeof(u32), "GLuint should be u32");

  ShaderProgram sceneShaderProgram{};
  sceneShaderProgram.create(
    readContentFromFile("src/shaders/sceneShader.vs"), readContentFromFile("src/shaders/sceneShader.fs"));

  ShaderProgram mergeSceneShaderProgram{};
  mergeSceneShaderProgram.create(
    readContentFromFile("src/shaders/mergeSceneShader.vs"), readContentFromFile("src/shaders/mergeSceneShader.fs"));

  // Cube object
  SimpleMesh cubeMesh = createCubeMesh();

  Texture cubeTexture{};
  cubeTexture.createTextureFromFile("texture.jpg");

  std::vector<glm::mat4> cubesModelMatrices{};
  srand(time(0));
  regenerateRandomTransformationMatrices(cubesModelMatrices);

  // Quad mesh for final combining draw step
  SimpleMesh quadMesh = createQuadMesh();

  // Decal object
  Decal testDecal{};
  testDecal.projectionBoxMesh = createPrimitiveCube();
  testDecal.setPosition(glm::vec3(-0.5, 0.5, -1));
  testDecal.setScale(glm::vec3(0.5));
  testDecal.texture.createTextureFromFile("decal.jpg");

  // Cube mesh for displaying decals projection
  SimpleMesh wireframeCubeMesh = createPrimitiveCube();

  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // generate texture
  GLuint textureColorbuffer{};
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB, globalContext.windowWidth, globalContext.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // attach it to currently bound framebuffer object
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  // Create Renderbuffer for Depth and Stencil attachments
  /*GLuint renderBufferObject;
  glGenRenderbuffers(1, &renderBufferObject);
  glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, globalContext.windowWidth, globalContext.windowHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);*/

  // Attach created Renderbuffer to Framebuffer with Depth_Stencil attachment
  // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

  GLuint textureDepthBuffer{};
  glGenTextures(1, &textureDepthBuffer);
  glBindTexture(GL_TEXTURE_2D, textureDepthBuffer);
  glTexImage2D(GL_TEXTURE_2D,
    0,
    GL_DEPTH_COMPONENT,
    globalContext.windowWidth,
    globalContext.windowHeight,
    0,
    GL_DEPTH_COMPONENT,
    GL_FLOAT,
    nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthBuffer, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  while (!glfwWindowShouldClose(globalContext.window))
  {
    TimeManager::getInstance().updateTime();

    processInput(globalContext.window);

    if (glfwGetKey(globalContext.window, GLFW_KEY_ENTER) == GLFW_PRESS)
      regenerateRandomTransformationMatrices(cubesModelMatrices);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    {
      glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cubeTexture.bind();
    sceneShaderProgram.bind();
    sceneShaderProgram.setUniform("texture1", 0);

    const glm::mat4 projection = camera.GetProjectionMatrix(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    const glm::mat4 view = camera.GetViewMatrix();

    sceneShaderProgram.setUniform("view", view);
    sceneShaderProgram.setUniform("projection", projection);

    glBindVertexArray(cubeMesh.vao);
    for (const glm::mat4 &cubeModel : cubesModelMatrices)
    {
      sceneShaderProgram.setUniform("model", cubeModel);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Render decals
    // {
    //   // Draw decal's projection volume
    //   glBindVertexArray(testDecal.projectionBoxMesh.vao);
    //   glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "model"),
    //     1,
    //     GL_FALSE,
    //     glm::value_ptr(testDecal.transform.getTransformMatrix()));

    //   glDrawArrays(GL_LINES, 0, 36);


    //   testDecal.texture.bind();
    //   testDecal.transform.getTransformMatrix();

    //   glUniform1i(glGetUniformLocation(sceneShaderProgram, "texture1"), 0);
    // }

    // Render final result (into default framebuffer)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      mergeSceneShaderProgram.bind();
      glBindVertexArray(quadMesh.vao);
      glDisable(GL_DEPTH_TEST);

      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      mergeSceneShaderProgram.setUniform("renderedSceneTexture", 0);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glfwSwapBuffers(globalContext.window);
    glfwPollEvents();
  }

  deleteSimpleMesh(cubeMesh);
  deleteSimpleMesh(quadMesh);
  deletePrimitiveCube(testDecal.projectionBoxMesh);

  sceneShaderProgram.destroy();

  glfwTerminate();
  return 0;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;// reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const bool accelerationEnabled = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, TimeManager::getDeltaTime(), accelerationEnabled);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, TimeManager::getDeltaTime(), accelerationEnabled);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, TimeManager::getDeltaTime(), accelerationEnabled);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, TimeManager::getDeltaTime(), accelerationEnabled);
}
