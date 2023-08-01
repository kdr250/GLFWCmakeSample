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
    /** ウィンドウ */
    GLFWwindow* window;

    /** ウィンドウのサイズ */
    GLfloat size[2];

    /** ワールド座標系に対するデバイス座標系の拡大率 */
    GLfloat scale;

public:
    Window(int width = 640, int height = 480, std::string title = "Hello") :
        window(glfwCreateWindow(width, height, title.c_str(), NULL, NULL)), scale(100.0f)
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

        glfwSetWindowUserPointer(window, this);

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

        Window* instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    const GLfloat* getSize() const
    {
        return size;
    }

    GLfloat getScale() const
    {
        return scale;
    }
};
