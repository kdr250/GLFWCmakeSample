#pragma once
#include "Shape.h"

/**
 * インデックスを使った図形の描画
 */
class ShapeIndex : public Shape
{
protected:
    /** 描画に使う頂点の数 */
    const GLsizei indexcount;

public:
    /**
     * @brief Construct a new ShapeIndex object
     *
     * @param size 頂点の位置の次元
     * @param vertexcount 頂点の数
     * @param vertex 頂点属性を格納した配列
     * @param indexcount 頂点のインデックスの要素数
     * @param index 頂点のインデックスを格納した配列
     */
    ShapeIndex(GLint size,
               GLsizei vertexcount,
               const Object::Vertex* vertex,
               GLsizei indexcount,
               const GLuint* index) :
        Shape(size, vertexcount, vertex, indexcount, index),
        indexcount(indexcount)
    {
    }

    /** 描画の実行 */
    virtual void execute() const
    {
        // 線分群で描画する
        glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, 0);
    }
};
