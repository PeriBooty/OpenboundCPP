#include "AssetHandler.h"
#include "BinaryReader.h"

#include <vector>
#include <unordered_map>

static std::unordered_map<uint8_t, std::shared_ptr<sf::Texture>> textures;
static std::unordered_map<uint8_t, std::shared_ptr<sf::SoundBuffer>> sounds;
static std::unordered_map<uint8_t, std::shared_ptr<SBURB::Font>> fonts;
static std::map<uint8_t, std::string_view> paths;

namespace SBURB
{
    // Texture handling
    std::shared_ptr<sf::Texture> AssetHandler::GetTextureById(int texID)
    {
        if (texID >= textures.size())
            return nullptr;
        return textures[texID];
    }

    int AssetHandler::LoadTextureFromFile(const std::string &path)
    {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path))
        {
            texture.reset();
            return -1;
        }
        // textures.push_back(std::move(texture));
        textures.insert(std::pair(textures.size(), texture));
        return textures.size() - 1;
    }

    int AssetHandler::LoadTextureFromMemory(const void *data, size_t size)
    {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromMemory(data, size))
        {
            texture.reset();
            return -1;
        }
        textures.insert(std::pair(textures.size(), texture));
        return textures.size() - 1;
    }

    void AssetHandler::ClearTextures()
    {
        for (int i = textures.size() - 1; i >= 0; i--)
        {
            textures[i].reset();
        }

        textures.clear();
    }

    // Sound handling
    std::shared_ptr<sf::SoundBuffer> AssetHandler::GetSoundById(int soundID)
    {
        if (soundID >= sounds.size())
            return nullptr;
        return sounds[soundID];
    }

    int AssetHandler::LoadSoundFromFile(const std::string &path)
    {
        for (auto it = paths.begin(); it != paths.end(); ++it)
        {
            if (it->second == path)
            {
                return it->first;
            }
        }

        std::shared_ptr<sf::SoundBuffer> buffer = std::make_shared<sf::SoundBuffer>();
        if (!buffer->loadFromFile(path))
        {
            GlobalLogger->Log(Logger::Error, "Failed to load sound from path: '" + path + "'");
            buffer.reset();
            return -1;
        }

        sounds.insert(std::pair(sounds.size(), buffer));
        paths.insert(std::pair(paths.size(), path));

        return sounds.size() - 1;
    }

    void AssetHandler::ClearSounds()
    {
        for (int i = sounds.size() - 1; i >= 0; i--)
        {
            sounds[i].reset();
        }

        sounds.clear();
        paths.clear();
    }

    // Font handling
    std::shared_ptr<Font> AssetHandler::GetFontById(int fontID)
    {
        if (fontID >= fonts.size())
            return nullptr;
        return fonts[fontID];
    }

    int AssetHandler::LoadFontFromMemory(const void *data, size_t size)
    {
        std::unordered_map<char, Glyph> glyphs;
        BinaryBufferReader bbr = BinaryBufferReader(&data, size);

        void *texData;
        size_t texSize;

        // Read glyph length
        uint8_t len = bbr.ReadUInt8();

        // Read glyphs
        for (int i = 0; i < len; i++)
        {
            // Read data
            uint8_t character = bbr.ReadUInt8();
            uint16_t x = bbr.ReadUInt16();
            uint16_t y = bbr.ReadUInt16();
            uint16_t w = bbr.ReadUInt16();
            uint16_t h = bbr.ReadUInt16();
            uint8_t shift = bbr.ReadUInt8();
            uint8_t offset = bbr.ReadUInt8();

            // Add glyph
            Glyph glyph;
            glyph.character = character;
            glyph.offset = offset;
            glyph.shift = shift;
            glyph.texture = sf::IntRect(x, y, w, h);

            glyphs.insert(std::pair<char, Glyph>(character, glyph));
        }

        // Read texture size
        texSize = bbr.ReadUInt64();

        // Read texture
        texData = 0; // TODO: Read texture, texture path, texture id or texture name

        // Create font
        std::shared_ptr<Font> font = std::make_shared<Font>();
        font->SetGlyphs(glyphs);
        font->SetTexId(AssetHandler::LoadTextureFromMemory(texData, texSize));

        fonts.insert(std::pair(fonts.size(), font));

        return fonts.size() - 1;
    }

    int AssetHandler::LoadFontFromFile(const std::string &texturePath, const std::string &path)
    {
        std::unordered_map<char, Glyph> glyphs;
        BinaryFileReader bfr = BinaryFileReader(path);

        // Read length
        uint8_t len = bfr.ReadUInt8();

        for (int i = 0; i < len; i++)
        {
            // Read data
            uint8_t character = bfr.ReadUInt8();
            uint16_t x = bfr.ReadUInt16();
            uint16_t y = bfr.ReadUInt16();
            uint16_t w = bfr.ReadUInt16();
            uint16_t h = bfr.ReadUInt16();
            uint8_t shift = bfr.ReadUInt8();
            uint8_t offset = bfr.ReadUInt8();

            // Add glyph
            Glyph glyph;
            glyph.character = character;
            glyph.offset = offset;
            glyph.shift = shift;
            glyph.texture = sf::IntRect(x, y, w, h);

            glyphs.insert(std::pair<char, Glyph>(character, glyph));
        }

        std::shared_ptr<Font> font = std::make_shared<Font>();
        font->SetGlyphs(glyphs);
        font->SetTexId(AssetHandler::LoadTextureFromFile(texturePath));

        fonts.insert(std::pair(fonts.size(), font));

        return fonts.size() - 1;
    }

    void AssetHandler::ClearFonts()
    {
        for (int i = fonts.size() - 1; i >= 0; i--)
        {
            fonts[i].reset();
        }

        fonts.clear();
    }
}