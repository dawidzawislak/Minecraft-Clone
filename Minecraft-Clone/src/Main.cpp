#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

#include <iostream>

bool InitOpenGLAndCreateWindow(GLFWwindow** window)
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    (*window) = glfwCreateWindow(800, 600, "Minecraft Clone", NULL, NULL);
    if (!(*window)) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return false;
    }
    return true;
}

int main()
{
    GLFWwindow* window = nullptr;

    if (!InitOpenGLAndCreateWindow(&window)) {
        std::cout << "Failed to init OpenGL and create window!";
        return -1;
    }
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    Texture texture("res/textures/grass.png");
    texture.Bind();

    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();
        
        renderer.Draw(va, ib, shader);
        

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    return 0;
}