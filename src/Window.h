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

    /** 図形の正規化デバイス座標系上での位置 */
    GLfloat location[2];

    /** キーボードの状態 */
    int keyStatus;

public:
    Window(int width = 640, int height = 480, std::string title = "Hello") :
        window(glfwCreateWindow(width, height, title.c_str(), NULL, NULL)),
        scale(100.0f), location {0.0f, 0.0f}, keyStatus(GLFW_RELEASE)
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

        glfwSetKeyCallback(window, keyboard);

        glfwSetScrollCallback(window, wheel);

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
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
        {
            location[0] -= 2.0f / size[0];
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
        {
            location[0] += 2.0f / size[0];
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
        {
            location[1] -= 2.0f / size[1];
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
        {
            location[1] += 2.0f / size[1];
        }

        // マウスの左ボタンが押されていたら...
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }

        return !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE);
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

    static void wheel(GLFWwindow* window, double x, double y)
    {
        Window* instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->scale += static_cast<GLfloat>(y);
        }
    }

    static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        Window* instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->keyStatus = action;
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

    const GLfloat* getLocation() const
    {
        return location;
    }
};
