#pragma once
#include <array>
#include "Matrix.h"

using Vector = std::array<GLfloat, 4>;

/**
 * @brief 行列とベクトルの乗算
 *
 * @param m Matrix型の行列
 * @param v Vector型のベクトル
 * @return Vector 乗算した結果
 */
Vector operator*(const Matrix& m, const Vector& v)
{
    Vector t;
    for (int i = 0; i < 4; i++)
    {
        t[i] = m[i] * v[0] + m[i + 4] * v[1] + m[i + 8] * v[2] + m[i + 12] * v[3];
    }
    return t;
}
