#include "Types.h"

#include "Camera.h"
#include "Decal.h"
#include "Framebuffer.h"
#include "RenderingContext.h"
#include "ShaderProgram.h"
#include "TimeManager.h"
#include "Utils.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <vector>

void processInput(GLFWwindow *window);

extern GlobalContext globalContext;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = DEFAULT_WINDOW_WIDTH / 2.0f;
float lastY = DEFAULT_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

Decal testDecal{};

int main()
{
  if (!createRenderingContext() || globalContext.window == nullptr)
  {
    return -1;
  }

  globalContext.camera = &camera;

  static_assert(sizeof(GLuint) == sizeof(u32), "GLuint should be u32");

  ShaderProgram sceneShaderProgram(
    readContentFromFile("src/shaders/sceneShader.vs"), readContentFromFile("src/shaders/sceneShader.fs"));

  ShaderProgram mergeSceneShaderProgram(
    readContentFromFile("src/shaders/mergeSceneShader.vs"), readContentFromFile("src/shaders/mergeSceneShader.fs"));

  ShaderProgram decalsShaderProgram(
    readContentFromFile("src/shaders/decalShader.vs"), readContentFromFile("src/shaders/decalShader.fs"));

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
  testDecal.projectionBoxMesh = createPrimitiveCube();
  testDecal.setPosition(glm::vec3(-0.5, 0.5, -1));
  testDecal.setScale(glm::vec3(0.5));
  testDecal.texture.createTextureFromFile("decal.jpg");

  Framebuffer sceneFBO{};
  sceneFBO.create(globalContext.windowWidth, globalContext.windowHeight);

  Framebuffer decalsFBO{};
  decalsFBO.create(globalContext.windowWidth, globalContext.windowHeight, false, true);

  // shader textures configuration
  // --------------------
  sceneShaderProgram.use();
  sceneShaderProgram.setUniform("texture1", 0);

  decalsShaderProgram.use();
  decalsShaderProgram.setUniform("depthMap", 0);

  mergeSceneShaderProgram.use();
  mergeSceneShaderProgram.setUniform("renderedSceneTexture", 0);

  testDecal.setScale(glm::vec3(0.3f, 0.3f, 1.0f));
  testDecal.setPosition(glm::vec3(-0.5, 0.5, -1));

  while (!glfwWindowShouldClose(globalContext.window))
  {
    TimeManager::getInstance().updateTime();

    processInput(globalContext.window);

    if (glfwGetKey(globalContext.window, GLFW_KEY_ENTER) == GLFW_PRESS)
      regenerateRandomTransformationMatrices(cubesModelMatrices);

    sceneFBO.bind();
    sceneFBO.clear(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), true, true);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    const glm::mat4 projection = camera.GetProjectionMatrix(globalContext.windowWidth, globalContext.windowHeight);
    const glm::mat4 view = camera.GetViewMatrix();

    sceneShaderProgram.use();
    sceneShaderProgram.setUniform("view", view);
    sceneShaderProgram.setUniform("projection", projection);

    cubeTexture.bind();

    glBindVertexArray(cubeMesh.vao);
    for (const glm::mat4 &cubeModel : cubesModelMatrices)
    {
      sceneShaderProgram.setUniform("model", cubeModel);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Render decals
    {
      // Draw decal's projection volume
      glBindVertexArray(testDecal.projectionBoxMesh.vao);

      const glm::mat4 decalModelMatrix = testDecal.transform.getTransformMatrix();
      sceneShaderProgram.use();
      sceneShaderProgram.setUniform("model", decalModelMatrix);
      glDrawArrays(GL_LINES, 0, 36);

      // Draw decals
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      decalsFBO.bind();
      decalsFBO.clear({ 0.7f, 0.7f, 0.7f, 1.0f }, false, true);

      // attaching scene color attachment to decals fbo
      decalsFBO.colorAttachment = sceneFBO.colorAttachment;
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, decalsFBO.colorAttachment, 0);

      decalsShaderProgram.use();

      float near_plane = 0.1f, far_plane = 10.1f;

      glm::mat4 decalProjection = glm::ortho(-0.15f, 0.15f, -0.15f, 0.15f, near_plane, far_plane);
      glm::vec3 decalEye = glm::vec3(testDecal.transform.position.x,
        testDecal.transform.position.y,
        testDecal.transform.position.z + (far_plane - near_plane) / 2.0f);
      glm::vec3 forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
      glm::mat4 decalView = glm::lookAt(decalEye, decalEye + forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f));

      const glm::mat4 viewProjection = projection * view;
      const glm::mat4 decalViewProjection = decalProjection * decalView;
      decalsShaderProgram.setUniform("view_projection", viewProjection);
      decalsShaderProgram.setUniform("decal_view_projection", decalViewProjection);
      decalsShaderProgram.setUniform("model", decalModelMatrix);

      glBindVertexArray(testDecal.projectionBoxMesh.vao);

      glBindTexture(GL_TEXTURE_2D, sceneFBO.depthAttachment);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Render final result (into default framebuffer)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      mergeSceneShaderProgram.use();
      glBindVertexArray(quadMesh.vao);
      glDisable(GL_DEPTH_TEST);

      glBindTexture(GL_TEXTURE_2D, sceneFBO.colorAttachment);
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


  const bool isDecalMovement =
    glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
    || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;

  if (isDecalMovement)
  {
    constexpr float movementSpeed = 5.1f;
    float velocity = movementSpeed * TimeManager::getDeltaTime();

    const glm::vec3 frontDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 rightDirection = glm::vec3(1.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      testDecal.setPosition(testDecal.transform.position + frontDirection * velocity);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      testDecal.setPosition(testDecal.transform.position - frontDirection * velocity);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
      testDecal.setPosition(testDecal.transform.position - rightDirection * velocity);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
      testDecal.setPosition(testDecal.transform.position + rightDirection * velocity);
  }
}
