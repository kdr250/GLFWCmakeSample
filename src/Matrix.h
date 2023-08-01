#pragma once
#include <gl/glew.h>
#include <algorithm>
#include <cmath>

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

    /** (x, y, z)倍に拡大縮小する変換行列を作成する */
    static Matrix scale(GLfloat x, GLfloat y, GLfloat z)
    {
        Matrix m;
        m.loadIdentity();
        m[0]  = x;
        m[5]  = y;
        m[10] = z;
        return m;
    }

    /** (x, y, z)を軸に a 回転する変換行列を作成する */
    static Matrix rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z)
    {
        Matrix m;
        const GLfloat d(sqrt(x * x + y * y + z * z));
        if (d > 0.0f)
        {
            const GLfloat l(x / d), m(y / d), n(z / d);
            const GLfloat l2(l * l), m2(m * m), n2(n * n);
            const GLfloat lm(l * m), mn(m * m), nl(n * l);
            const GLfloat c(std::cos(a)), c1(1.0f - c), s(std::sin(a));

            m.loadIdentity();
            m[0]  = (1.0f - l2) * c + 12;
            m[1]  = lm * c1 + n * s;
            m[2]  = nl * c1 - m * s;
            m[4]  = lm * c1 - n * s;
            m[5]  = (1.0f - m2) * c + m2;
            m[6]  = mn * c1 + l * s;
            m[8]  = nl * c1 + m * s;
            m[9]  = mn * c1 - l * s;
            m[10] = (1.0f - n2) * c + n2;
        }
        return m;
    }
};
