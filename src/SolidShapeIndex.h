#pragma once
#include "ShapeIndex.h"

/**
 * インデックスを使った三角形による描画
 */
class SolidShapeIndex : public ShapeIndex
{
public:
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
