#pragma once
#include "ShapeIndex.h"

/**
 * インデックスを使った三角形による描画
 */
class SolidShapeIndex : public ShapeIndex
{
public:
    /**
     * @brief Construct a new SolidShapeIndex object
     *
     * @param size 頂点の位置の次元
     * @param vertexcount 頂点の数
     * @param vertex 頂点属性を格納した配列
     * @param indexcount 頂点のインデックスの要素数
     * @param index 頂点のインデックスを格納した配列
     */
    SolidShapeIndex(GLint size,
                    GLsizei vertexcount,
                    const Object::Vertex* vertex,
                    GLsizei indexcount,
                    const GLuint* index) :
        ShapeIndex(size, vertexcount, vertex, indexcount, index)
    {
    }

    /** 描画の実行 */
    virtual void execute() const
    {
        // 三角形で描画する
        glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
    }
};
