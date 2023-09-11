#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

#include <iostream>

bool InitOpenGLAndCreateWindow(GLFWwindow** window);

void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ProcessKeyboardInput(GLFWwindow* window);

// settings
const unsigned int WND_WIDTH = 800;
const unsigned int WND_HEIGHT = 600;

// Camera stuff
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
float lastX = WND_WIDTH / 2.0f;
float lastY = WND_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    GLFWwindow* window = nullptr;

    if (!InitOpenGLAndCreateWindow(&window)) {
        std::cout << "Failed to init OpenGL and create window!";
        return -1;
    }
    

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);

    std::vector<Block> blocks;
    blocks.emplace_back(BlockType::GRASS, 0.0f, 0.0f, 0.0f);
    blocks.emplace_back(BlockType::DIRT, -2.0f, 0.0f, 0.0f);
    blocks.emplace_back(BlockType::STONE, 2.0f, 0.0f, 0.0f);

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        // --- Update ----------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ProcessKeyboardInput(window);

        view = camera.GetViewMatrix();
        glm::mat4 vp = proj * view;

        // --- Draw ------------
        renderer.Clear();

        renderer.DrawBlocks(blocks, vp);

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    return 0;
}

bool InitOpenGLAndCreateWindow(GLFWwindow** window)
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    (*window) = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, "Minecraft Clone", NULL, NULL);
    if (!(*window)) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);

    glfwSetCursorPosCallback(*window, MouseCallback);
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return false;
    }
    return true;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float fxPos = static_cast<float>(xPos);
    float fyPos = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = fxPos;
        lastY = fyPos;
        firstMouse = false;
    }

    float xOffset = fxPos - lastX;
    float yOffset = lastY - fyPos; // reversed since y-coordinates go from bottom to top

    lastX = fxPos;
    lastY = fyPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void ProcessKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Direction::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Direction::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Direction::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Direction::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Direction::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(Direction::DOWN, deltaTime);
}