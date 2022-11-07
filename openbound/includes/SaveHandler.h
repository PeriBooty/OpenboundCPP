#ifndef SBURB_SAVE_HANDLER_H
#define SBURB_SAVE_HANDLER_H
#include <string>
#include <unordered_map>
#include <variant>

namespace SBURB
{
    struct DataType
    {
        enum class ValueType
        {
            valtype_string,
            valtype_double,
            valtype_int64
        };

        ValueType type;
        std::variant<std::string, double, int64_t> variant;

        DataType() : type(DataType::ValueType::valtype_string) {}
        DataType(std::string variant) : type(DataType::ValueType::valtype_string), variant(variant) {}
        DataType(double variant) : type(DataType::ValueType::valtype_double), variant(variant) {}
        DataType(int64_t variant) : type(DataType::ValueType::valtype_int64), variant(variant) {}
    };

    enum class FileEncoding
    {
        Standard,
        Binary
    };

    // Class used for saving and loading data
    class SaveHandler
    {
    public:
        SaveHandler();

        static void SaveData(std::string filepath, std::unordered_map<std::string, DataType> data, FileEncoding encoding = FileEncoding::Standard);
        static bool DeleteFile(std::string filepath);
        static std::unordered_map<std::string, DataType> LoadData(std::string filepath, FileEncoding encoding = FileEncoding::Standard);

        static void Reset();

    private:
        std::string basePath;
    };
}
#endif