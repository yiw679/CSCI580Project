#include "Input.h"
#include "Terrain.h"
#include "Time.h"
#include "Camera.h"
#include "Shader.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Real Scene Rendering and Interaction", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    ///////////
    Time* time = new Time();

    Shader shader("Source/shader.vs", "Source/shader.fs");

    Camera cam = Camera(time);

    Input::getInstance().SetCamera(cam);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    int map_size = 128;

    Terrain NewTerrain(map_size + 1, 0.1, 0.01);

    NewTerrain.Generate(100, 0.6);

    NewTerrain.LoadTexture("Textures/grass.jpg", 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        shader.Enable();
        shader.setFloat("tileSize", (3 / (float)map_size));
        glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9, 0.1f, 100.0f);

        shader.setMat4("projection", projection);
        shader.setMat4("view", Input::getInstance().cam.getPosition());
        shader.setMat4("model", NewTerrain.m_Xmw);

        NewTerrain.Render();

        time->update();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}