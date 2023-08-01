#pragma once
#include <gl/glew.h>
#include <algorithm>

/**
 * 変換行列のクラス
 */
class Matrix
{
    /** 変換行列の要素 */
    GLfloat matrix[16];

public:
    Matrix() {}

    /**
     * @brief 配列の内容で初期化するコンストラクタ
     *
     * @param a GLfloat型の16要素の配列
     */
    Matrix(const GLfloat* a)
    {
        std::copy(a, a + 16, matrix);
    }

    /** 配列の要素を右辺値として参照する */
    const GLfloat& operator[](std::size_t i) const
    {
        return matrix[i];
    }

    /** 配列の要素を左辺値として参照する */
    GLfloat& operator[](std::size_t i)
    {
        return matrix[i];
    }
};
