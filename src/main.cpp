#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "Shape.h"

/** 矩形の頂点の位置 */
constexpr Object::Vertex rectangleVertex[] = {{-0.5f, -0.5f},
                                              {0.5f, -0.5f},
                                              {0.5f, 0.5f},
                                              {-0.5f, 0.5f}};

/**
 * @brief print the error log of shader compile
 *
 * @param shader
 * @param str
 * @return GLboolean
 */
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Compile Error in " << str << std::endl;

    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1)
    {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
}

/**
 * @brief print the error log of shader compile
 *
 * @param program
 * @return GLboolean
 */
GLboolean printProgramInfoLog(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Link Error." << std::endl;

    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1)
    {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
}

/**
 * @brief Create a Program object
 *
 * @param vsrc
 * @param fsrc
 * @return GLuint
 */
GLuint createProgram(const std::string& vsrc, const std::string& fsrc)
{
    const GLuint program(glCreateProgram());

    const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
    auto charVsrc = vsrc.c_str();
    glShaderSource(vobj, 1, &charVsrc, NULL);
    glCompileShader(vobj);

    if (printShaderInfoLog(vobj, "vertex shader"))
    {
        glAttachShader(program, vobj);
    }
    glDeleteShader(vobj);

    const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
    auto charFsrc = fsrc.c_str();
    glShaderSource(fobj, 1, &charFsrc, NULL);
    glCompileShader(fobj);

    if (printShaderInfoLog(fobj, "fragment shader"))
    {
        glAttachShader(program, fobj);
    }
    glDeleteShader(fobj);

    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    if (printProgramInfoLog(program))
        return program;

    glDeleteProgram(program);
    return 0;
}

/**
 * @brief read shader file
 *
 * @param name
 * @param result
 * @return true
 * @return false
 */
bool readShaderSource(const std::string name, std::string& result)
{
    std::ifstream file(name);
    std::stringstream ss;

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << name << std::endl;
        file.close();
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        ss << line << std::endl;
    }

    result = ss.str();
    file.close();
    return true;
}

/**
 * @brief load shader program from file
 *
 * @param vert
 * @param frag
 * @return GLuint
 */
GLuint loadProgram(std::string vert, std::string frag)
{
    std::string vsrc;
    const bool vstat(readShaderSource(vert, vsrc));

    std::string fsrc;
    const bool fstat(readShaderSource(frag, fsrc));

    return vstat && fstat ? createProgram(vsrc, fsrc) : 0;
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

    glViewport(100, 50, 300, 300);

    const GLuint program(loadProgram("resources/point.vert", "resources/point.frag"));

    // 図形を作成する
    std::unique_ptr<const Shape> shape = std::make_unique<const Shape>(2, 4, rectangleVertex);

    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        // 図形を描画
        shape->draw();

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
