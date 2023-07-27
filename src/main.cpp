#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

int main()
{
    // initialize GLFW
    if (glfwInit() == GL_FALSE)
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    atexit(glfwTerminate);

    // initialize window
    GLFWwindow* window(glfwCreateWindow(640, 480, "Sample", NULL, NULL));
    if (window == NULL)
    {
        std::cerr << "Failed to initialize window" << std::endl;
        return 1;
    }

    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return 1;
    }

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Not Yet Implemented

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
