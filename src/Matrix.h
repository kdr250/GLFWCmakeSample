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

    /** 変換行列の配列を返す */
    const GLfloat* data() const
    {
        return matrix;
    }

    /** 単位行列を設定する */
    void loadIdentity()
    {
        std::fill(matrix, matrix + 16, 0.0f);
        matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
    }

    /** 単位行列を作成する */
    static Matrix identity()
    {
        Matrix m;
        m.loadIdentity();
        return m;
    }

    /** (x, y, z)だけ平行移動する変換行列を作成する */
    static Matrix translate(GLfloat x, GLfloat y, GLfloat z)
    {
        Matrix m;
        m.loadIdentity();
        m[12] = x;
        m[13] = y;
        m[14] = z;
        return m;
    }
};
