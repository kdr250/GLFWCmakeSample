#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

/**
 * ウィンドウ関連の処理を扱うクラス
 */
class Window
{
    GLFWwindow* window;

public:
    Window(int width = 640, int height = 480, std::string title = "Hello") :
        window(glfwCreateWindow(width, height, title.c_str(), NULL, NULL))
    {
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            exit(1);
        }

        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            exit(1);
        }

        glfwSwapInterval(1);

        glfwSetWindowSizeCallback(window, resize);

        resize(window, width, height);
    }

    virtual ~Window()
    {
        glfwDestroyWindow(window);
    }

    explicit operator bool()
    {
        glfwWaitEvents();

        return !glfwWindowShouldClose(window);
    }

    void swapBuffers() const
    {
        glfwSwapBuffers(window);
    }

    static void resize(GLFWwindow* window, int width, int height)
    {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        glViewport(0, 0, fbWidth, fbHeight);
    }
};
