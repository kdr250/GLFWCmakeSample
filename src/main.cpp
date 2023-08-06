#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShape.h"
#include "SolidShapeIndex.h"
#include "Window.h"

/** 面ごとに法線を変えた六面体の頂点属性 */
constexpr Object::Vertex solidCubeVertex[] = {
    // 左
    // 左
    {-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
    // 裏
    {1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f},
    {1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f},
    {1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f},
    // 下
    {-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f},
    {-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f},
    {-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f},
    // 右
    {1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
    // 上
    {-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    // 前
    {-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}};

/** 六面体の面を塗りつぶす三角形の頂点のインデックス */
constexpr GLuint solidCubeIndex[] = {
    0,  1,  2,  3,  4,  5,   // 左
    6,  7,  8,  9,  10, 11,  // 裏
    12, 13, 14, 15, 16, 17,  // 下
    18, 19, 20, 21, 22, 23,  // 右
    24, 25, 26, 27, 28, 29,  // 上
    30, 31, 32, 33, 34, 35   // 前
};

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
    glBindAttribLocation(program, 1, "normal");
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
    Window window;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // 背面カリングを有効にする
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // デプスバッファを有効にする
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    const GLuint program(loadProgram("resources/point.vert", "resources/point.frag"));
    const GLint projectionLocation(glGetUniformLocation(program, "projection"));
    const GLint modelViewLocation(glGetUniformLocation(program, "modelView"));

    // 図形を作成する
    std::unique_ptr<const Shape> shape =
        std::make_unique<const SolidShapeIndex>(3, 36, solidCubeVertex, 36, solidCubeIndex);

    glfwSetTime(0.0);

    while (window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        // 透視投影変換行列を求める
        const GLfloat* size(window.getSize());
        const GLfloat fovy(window.getScale() * 0.01f);
        const GLfloat aspect(size[0] / size[1]);
        const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

        // モデルの変換行列を求める
        const GLfloat* location(window.getLocation());
        const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f));
        const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);

        // ビュー変換行列を求める
        const Matrix view(Matrix::lookAt(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
        // モデルビュー変換行列を求める
        const Matrix modelView(view * model);
        // uniform 変数に値を設定する
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelView.data());

        // 図形を描画
        shape->draw();

        // 二つ目のモデルビュー変換行列を求める
        const Matrix modelview1(modelView * Matrix::translate(0.0f, 0.0f, 3.0f));
        // uniform 変数に値を設定する
        glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelview1.data());
        // 二つ目の図形を描画する
        shape->draw();

        window.swapBuffers();
    }

    return 0;
}
