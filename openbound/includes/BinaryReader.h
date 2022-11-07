#ifndef SBURB_BINARY_READER_H
#define SBURB_BINARY_READER_H

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
    // class Buffer;
    class BinaryReader
    {
    public:
        uint8_t ReadUInt8();
        uint16_t ReadUInt16();
        uint32_t ReadUInt32();
        uint64_t ReadUInt64();

        int8_t ReadInt8();
        int16_t ReadInt16();
        int32_t ReadInt32();
        int64_t ReadInt64();

        float ReadFloat();
        double ReadDouble();

        std::string ReadString();
        Buffer ReadBuffer();
        Buffer ReadBuffer(uint32_t length);
        Buffer ReadCompressedBuffer();
        Buffer ReadCompressedBuffer(uint32_t uncompressedLength);
        Buffer ReadCompressedBuffer(uint32_t uncompressedLength, uint32_t length);

        bool CheckMagic(uint32_t magic);

    private:
        virtual int Read(void *ptr, size_t size) = 0;

    protected:
        int endian = LITTLE_ENDIAN;
    };

    class BinaryFileReader : public BinaryReader
    {
    public:
        BinaryFileReader(std::string filePath);
        ~BinaryFileReader();

        bool CanRead();
        bool IsAtEOF();

    private:
        FILE *fd;
        bool atEOF;
        size_t fileSize;
        bool canRead;
        virtual int Read(void *ptr, size_t size);
    };

    class BinaryBufferReader : public BinaryReader
    {
    public:
        BinaryBufferReader(Buffer buffer);
        BinaryBufferReader(void *ptr, size_t size);

        bool CanRead();
        bool IsAtEOF();

    private:
        char *buffer;
        bool atEOF;
        size_t bufferPos;
        size_t bufferSize;
        bool canRead;
        virtual int Read(void *ptr, size_t size);
    };

    // TODO: BinaryBufferReader
}

#endif