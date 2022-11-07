#ifndef SBURB_SHADER_H
#define SBURB_SHADER_H

#include <string>

namespace SBURB
{
    class Shader
    {
    public:
        Shader(const std::string &vertPath, const std::string &fragPath);
        Shader();
        ~Shader();

        template <typename T>
        void SetUniform(const std::string &name, const T &value);

        void Bind() const;
        void Unbind() const;

        inline explicit operator unsigned int() const
        {
            return glID;
        }

    private:
        unsigned int glID;
    };
}

#endif