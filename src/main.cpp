#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

/**
 * @brief Create a Program object
 *
 * @param vsrc
 * @param fsrc
 * @return GLuint
 */
GLuint createProgram(const char* vsrc, const char* fsrc)
{
    const GLuint program(glCreateProgram());

    if (vsrc != NULL)
    {
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);

        glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }

    if (fsrc != NULL)
    {
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);

        glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }

    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    return program;
}

int main()
{
    // initialize GLFW
    if (glfwInit() == GL_FALSE)
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    atexit(glfwTerminate);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glfwSwapInterval(1);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    static constexpr GLchar vsrc[] =
        "#version 150 core¥n"
        "in vec4 position;¥n"
        "void main()¥n"
        "{¥n"
        " gl_Position = position;¥n"
        "}¥n";

    static constexpr GLchar fsrc[] =
        "#version 150 core¥n"
        "out vec4 fragment;¥n"
        "void main()¥n"
        "{¥n"
        " fragment = vec4(1.0, 0.0, 0.0, 1.0);¥n"
        "}¥n";

    const GLuint program(createProgram(vsrc, fsrc));

    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        // TODO: Not Yet Implemented

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
