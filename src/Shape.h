#pragma once
#include <memory>
#include "Object.h"

/**
 * 図形の描画を行うクラス
 */
class Shape
{
    /** 図形データ */
    std::shared_ptr<const Object> object;

protected:
    /** 描画に使う頂点の数 */
    const GLsizei vertexcount;

public:
    /**
     * @brief Construct a new Shape object
     *
     * @param size 頂点の位置の次元
     * @param vertexcount 頂点の数
     * @param vertex 頂点属性を格納した配列
     */
    Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex)
        : object(new Object(size, vertexcount, vertex)), vertexcount(vertexcount)
    {}

    /** 描画する */
    void draw() const
    {
        object->bind();
        execute();
    }

    /** 描画を実行する */
    virtual void execute() const
    {
        // 折線で描画する
        glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
    }
};
