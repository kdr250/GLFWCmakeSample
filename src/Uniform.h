#pragma once
#include <GL/glew.h>
#include <memory>

/**
 * ユニフォームバッファオブジェクト
 */
template<typename T>
class Uniform
{
    struct UniformBuffer
    {
        /** ユニフォームバッファオブジェクト名 */
        GLuint ubo;

        /** ユニフォームブロックのサイズ */
        GLsizeiptr blocksize;

        /**
         * @brief Construct a new Uniform Buffer object
         *
         * @param data uniformブロックに格納するデータ
         * @param count 確保するuniformブロックの数
         */
        UniformBuffer(const T* data, unsigned int count)
        {
            // ユニフォームブロックのサイズを求める
            GLint alignment;
            glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
            blocksize = (((sizeof(T) - 1) / alignment) + 1) * alignment;
            glGenBuffers(1, &ubo);
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferData(GL_UNIFORM_BUFFER, count * blocksize, NULL, GL_STATIC_DRAW);
            for (unsigned int i = 0; i < count; ++i)
            {
                glBufferSubData(GL_UNIFORM_BUFFER, i * blocksize, sizeof(T), data + i);
            }
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
     * @param count 確保するuniformブロックの数
     */
    Uniform(const T* data = NULL, unsigned int count = 1) : buffer(new UniformBuffer(data, count))
    {
    }

    virtual ~Uniform() {}

    /** ユニフォームバッファオブジェクトにデータを格納する */
    void set(const T* data, unsigned int start = 0, unsigned int count = 1) const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
        for (unsigned int i = 0; i < count; ++i)
        {
            glBufferSubData(GL_UNIFORM_BUFFER,
                            (start + i) * buffer->blocksize,
                            sizeof(T),
                            data + i);
        }
    }

    /** ユニフォームバッファオブジェクトを使用する */
    void select(GLuint bp, unsigned int i = 0) const
    {
        // 材質に設定するユニフォームバッファオブジェクトを指定する
        glBindBufferRange(GL_UNIFORM_BUFFER, bp, buffer->ubo, i * buffer->blocksize, sizeof(T));
    }
};
