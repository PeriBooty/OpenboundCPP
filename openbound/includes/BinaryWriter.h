#ifndef SBURB_BINARY_WRITER_H
#define SBURB_BINARY_WRITER_H

#include "Buffer.h"

#include <stdint.h>
#include <stdio.h>
#include <string>

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#endif

namespace SBURB
{
    class BinaryWriter
    {
    public:
        BinaryWriter();

        void WriteUInt8(uint8_t value);
        void WriteUInt16(uint16_t value);
        void WriteUInt32(uint32_t value);
        void WriteUInt64(uint64_t value);

        void WriteInt8(int8_t value);
        void WriteInt16(int16_t value);
        void WriteInt32(int32_t value);
        void WriteInt64(int64_t value);

        void WriteFloat(float value);
        void WriteDouble(double value);

        void WriteString(std::string value);
        void WriteBuffer(Buffer value);
        void WriteCompressedBuffer(Buffer value);

    private:
        virtual int Write(const void *ptr, size_t size) = 0;

    protected:
        int endian = LITTLE_ENDIAN;
    };

    class BinaryFileWriter : public BinaryWriter
    {
    public:
        BinaryFileWriter(std::string filePath);
        ~BinaryFileWriter();

        bool CanWrite();

    private:
        FILE *fd;
        bool canWrite;
        virtual int Write(const void *ptr, size_t size);
    };

    // TODO: BinaryBufferWriter
}

#endif