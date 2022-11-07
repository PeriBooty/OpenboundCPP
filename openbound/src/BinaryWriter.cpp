#include "BinaryWriter.h"
#include "Common.h"
#include "Logger.h"

#include <cstring>

namespace SBURB
{
    BinaryWriter::BinaryWriter()
    {
        uint16_t number = 0x1;
        char *numPtr = (char *)&number;
        this->endian = (numPtr[0] == 1) ? LITTLE_ENDIAN : BIG_ENDIAN;
    }

    void BinaryWriter::WriteUInt8(uint8_t value)
    {
        Write(&value, 1);
    }

    void BinaryWriter::WriteUInt16(uint16_t value)
    {
        Write(&value, 2);
    }

    void BinaryWriter::WriteUInt32(uint32_t value)
    {
        Write(&value, 4);
    }

    void BinaryWriter::WriteUInt64(uint64_t value)
    {
        Write(&value, 8);
    }

    void BinaryWriter::WriteInt8(int8_t value)
    {
        Write(&value, 1);
    }

    void BinaryWriter::WriteInt16(int16_t value)
    {
        Write(&value, 2);
    }

    void BinaryWriter::WriteInt32(int32_t value)
    {
        Write(&value, 4);
    }

    void BinaryWriter::WriteInt64(int64_t value)
    {
        Write(&value, 8);
    }

    void BinaryWriter::WriteFloat(float value)
    {
        Write(&value, sizeof(value));
    }

    void BinaryWriter::WriteDouble(double value)
    {
        Write(&value, sizeof(value));
    }

    void BinaryWriter::WriteString(std::string value)
    {
        if (value[value.size() - 1] != '\0')
        {
            value.push_back('\0');
        }

        for (int i = 0; i < value.size(); i++)
        {
            Write(&(value[i]), 1);
        }
    }

    void BinaryWriter::WriteBuffer(Buffer value)
    {
        uint32_t length = value.GetLength();
        WriteUInt32(length);
        Write(value.GetData(), length);
    }

    void BinaryWriter::WriteCompressedBuffer(Buffer value)
    {
        WriteUInt32(value.GetLength());
        value.Compress();
        WriteBuffer(value);
    }

    BinaryFileWriter::BinaryFileWriter(std::string filePath)
        : BinaryWriter()
    {
        fd = fopen(filePath.c_str(), "wb");
        if (fd == NULL)
        {
            canWrite = false;
            GlobalLogger->Log(Logger::Error, "Failed to open file for writing at path '" + filePath + "', error: " + std::string(strerror(errno)));
        }
        else
        {
            canWrite = true;
        }
    }

    BinaryFileWriter::~BinaryFileWriter()
    {
        if (canWrite)
            fclose(this->fd);
    }

    bool BinaryFileWriter::CanWrite()
    {
        return canWrite;
    }

    int BinaryFileWriter::Write(const void *ptr, size_t size)
    {
        if (!canWrite)
            return 0;
        if (this->endian == BIG_ENDIAN)
        {
            switch (size)
            {
            case 1:
                return (int)fwrite(ptr, size, 1, fd);
            case 2:
            {
                uint16_t res = *((uint16_t *)ptr);
                res = swapbits(res);
                return (int)fwrite(&res, size, 1, fd);
            }
            case 4:
            {
                uint32_t res = *((uint32_t *)ptr);
                res = swapbits(res);
                return (int)fwrite(&res, size, 1, fd);
            }
            case 8:
            {
                uint64_t res = *((uint32_t *)ptr);
                res = swapbits(res);
                return (int)fwrite(&res, size, 1, fd);
            }
            default:
                return (int)fwrite(ptr, size, 1, fd);
            }
        }
        return (int)fwrite(ptr, size, 1, fd);
    }
}