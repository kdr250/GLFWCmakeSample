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

    /** 乗算 */
    Matrix operator*(const Matrix& other) const
    {
        Matrix m;
        for (int i = 0; i < 16; i++)
        {
            int j(i & 3), k(i & ~3);

            m[i] = matrix[0 + j] * other[k + 0] + matrix[4 + j] * other[k + 1]
                   + matrix[8 + j] * other[k + 2] + matrix[12 + j] * other[k + 3];
        }
        return m;
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
        Matrix mat;
        const GLfloat d(sqrt(x * x + y * y + z * z));
        if (d > 0.0f)
        {
            const GLfloat l(x / d), m(y / d), n(z / d);
            const GLfloat l2(l * l), m2(m * m), n2(n * n);
            const GLfloat lm(l * m), mn(m * m), nl(n * l);
            const GLfloat c(std::cos(a)), c1(1.0f - c), s(std::sin(a));

            mat.loadIdentity();
            mat[0]  = (1.0f - l2) * c + l2;
            mat[1]  = lm * c1 + n * s;
            mat[2]  = nl * c1 - m * s;
            mat[4]  = lm * c1 - n * s;
            mat[5]  = (1.0f - m2) * c + m2;
            mat[6]  = mn * c1 + l * s;
            mat[8]  = nl * c1 + m * s;
            mat[9]  = mn * c1 - l * s;
            mat[10] = (1.0f - n2) * c + n2;
        }
        return mat;
    }

    /**
     * @brief ビュー変換行列を作成する
     *
     * @param ex, ey, ez 視点の位置
     * @param gx, gy, gz 目標点の位置
     * @param ux, uy, uz 上方向のベクトル
     */
    static Matrix lookAt(GLfloat ex,
                         GLfloat ey,
                         GLfloat ez,
                         GLfloat gx,
                         GLfloat gy,
                         GLfloat gz,
                         GLfloat ux,
                         GLfloat uy,
                         GLfloat uz)
    {
        // 平行移動の変換行列
        const Matrix tv(translate(-ex, -ey, -ez));

        // t 軸 = e - g
        const GLfloat tx(ex - gx);
        const GLfloat ty(ey - gy);
        const GLfloat tz(ez - gz);
        // r 軸 = u x t 軸
        const GLfloat rx(uy * tz - uz * ty);
        const GLfloat ry(uz * tx - ux * tz);
        const GLfloat rz(ux * ty - uy * tx);
        // s 軸 = t 軸 x r 軸
        const GLfloat sx(ty * rz - tz * ry);
        const GLfloat sy(tz * rx - tx * rz);
        const GLfloat sz(tx * ry - ty * rx);

        // s 軸の長さのチェック
        const GLfloat s2(sx * sx + sy * sy + sz * sz);
        if (s2 == 0.0f)
            return tv;
        // 回転の変換行列
        Matrix rv;
        rv.loadIdentity();
        // r 軸を正規化して配列変数に格納
        const GLfloat r(sqrt(rx * rx + ry * ry + rz * rz));
        rv[0] = rx / r;
        rv[4] = ry / r;
        rv[8] = rz / r;
        // s 軸を正規化して配列変数に格納
        const GLfloat s(sqrt(s2));
        rv[1] = sx / s;
        rv[5] = sy / s;
        rv[9] = sz / s;
        // t 軸を正規化して配列変数に格納
        const GLfloat t(sqrt(tx * tx + ty * ty + tz * tz));
        rv[2]  = tx / t;
        rv[6]  = ty / t;
        rv[10] = tz / t;

        // 視点の平行移動の変換行列に視線の回転の変換行列を乗じる
        return rv * tv;
    }

    /** 直交投影変換行列を作成する */
    static Matrix orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
    {
        Matrix m;
        const GLfloat dx(right - left);
        const GLfloat dy(top - bottom);
        const GLfloat dz(zFar - zNear);

        if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
        {
            m.loadIdentity();
            m[0] = 2.0f / dx;
            m[5] = 2.0f / dy;
            m[10] = -2.0f / dz;
            m[12] = -(right + left) / dx;
            m[13] = -(top + bottom) / dy;
            m[14] = -(zFar + zNear) / dz;
        }

        return m;
    }

    /** 透視投影変換行列を作成する */
    static Matrix frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
    {
        Matrix m;
        const GLfloat dx(right - left);
        const GLfloat dy(top - bottom);
        const GLfloat dz(zFar - zNear);

        if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
        {
            m.loadIdentity();
            m[0] = 2.0f * zNear / dx;
            m[5] = 2.0f * zNear / dy;
            m[8] = (right + left) / dx;
            m[9] = (top + bottom) / dy;
            m[10] = -(zFar + zNear) / dz;
            m[11] = -1.0f;
            m[14] = -2.0f * zFar * zNear / dz;
            m[15] = 0.0f;
        }

        return m;
    }

    /** 画角を指定して透視投影変換行列を作成する */
    static Matrix perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
    {
        Matrix m;
        const GLfloat dz(zFar - zNear);

        if (dz != 0.0f)
        {
            m.loadIdentity();
            m[5] = 1.0f / std::tan(fovy * 0.5f);
            m[0] = m[5] / aspect;
            m[10] = -(zFar + zNear) / dz;
            m[11] = -1.0f;
            m[14] = -2.0f * zFar * zNear / dz;
            m[15] = 0.0f;
        }

        return m;
    }
};
