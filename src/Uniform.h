#pragma once
#include <memory>
#include <GL/glew.h>

/**
 * ユニフォームバッファオブジェクト
 */
template <typename T>
class Uniform
{
    struct UniformBuffer
    {
        /** ユニフォームバッファオブジェクト名 */
        GLuint ubo;

        /**
         * @brief Construct a new Uniform Buffer object
         *
         * @param data uniformブロックに格納するデータ
         */
        UniformBuffer(const T* data)
        {
            glGenBuffers(1, &ubo);
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(T), data, GL_STATIC_DRAW);
        }

        ~UniformBuffer()
        {
            glDeleteBuffers(1, &ubo);
        }
    };

    const std::shared_ptr<const UniformBuffer> buffer;

public:
    /**
     * @brief Construct a new Uniform object
     *
     * @param data uniformブロックに格納するデータ
     */
    Uniform(const T* data = NULL) : buffer(new UniformBuffer(data))
    {
    }

    virtual ~Uniform()
    {
    }

    /** ユニフォームバッファオブジェクトにデータを格納する */
    void set(const T* data) const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), data);
    }

    /** ユニフォームバッファオブジェクトを使用する */
    void select(GLuint bp) const
    {
        // 材質に設定するユニフォームバッファオブジェクトを指定する
        glBindBufferBase(GL_UNIFORM_BUFFER, bp, buffer->ubo);
    }
};
