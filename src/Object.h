#pragma once
#include <GL/glew.h>

/**
 * 頂点配列オブジェクトのクラス
 */
class Object
{
    /** 頂点配列オブジェクト名 */
    GLuint vao;
    /** 頂点バッファオブジェクト名 */
    GLuint vbo;
    /** インデックスの頂点バッファオブジェクト */
    GLuint ibo;

public:
    /**
     * 頂点属性を表す構造体
     */
    struct Vertex
    {
        /** 位置 */
        GLfloat position[3];
        /**色 */
        GLfloat color[3];
    };

    /**
     * @brief Construct a new Object object
     *
     * @param size 頂点の位置の次元
     * @param vertexcount 頂点の数
     * @param vertex 頂点属性を格納した配列
     * @param indexcount 頂点のインデックスの要素数
     * @param index 頂点のインデックスを格納した配列
     */
    Object(GLint size,
           GLsizei vertexcount,
           const Vertex* vertex,
           GLsizei indexcount  = 0,
           const GLuint* index = NULL)
    {
        // 頂点配列オブジェクト
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // 頂点バッファオブジェクト
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

        // 結合されている頂点バッファオブジェクトをin変数から参照できるようにする
        glVertexAttribPointer(0,
                              size,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              static_cast<Vertex*>(0)->position);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              static_cast<Vertex*>(0)->color);
        glEnableVertexAttribArray(1);

        // インデックスの頂点バッファオブジェクト
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
    }

    virtual ~Object()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }

    /** 頂点配列オブジェクトを結合する */
    void bind() const
    {
        // 描画する頂点配列オブジェクトを指定する
        glBindVertexArray(vao);
    }

private:
    /** コピーコンストラクタによるコピー禁止 */
    Object(const Object& o);

    /** 代入によるコピー禁止 */
    Object& operator=(const Object& o);
};
