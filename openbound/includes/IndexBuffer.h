#ifndef SBURB_INDEX_BUFFER_H
#define SBURB_INDEX_BUFFER_H

namespace SBURB
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const unsigned int *data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return count; }

    private:
        unsigned int glID;
        unsigned int count;
    };
}

#endif