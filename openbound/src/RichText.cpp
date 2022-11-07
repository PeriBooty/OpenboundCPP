#include "RichText.h"
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <iostream>

namespace SBURB
{
    RichText::RichText()
    {
        this->font = NULL;
        this->rawText = "";
        this->renderPosition = {0, 0};
        this->monospacing = -1;
        this->renderOffset = {0, 0};
        this->ignoreTags = false;
        this->wavyAngle = 0;
        this->scale = 1.0;
        this->textTypeFlags = static_cast<char>(TextType::Normal);

        this->colorPresets = std::unordered_map<std::string, int32_t>();
        colorPresets.insert(std::pair<std::string, int32_t>("Yellow", 0xFFFF00FF));
        colorPresets.insert(std::pair<std::string, int32_t>("Black", 0x000000FF));
        colorPresets.insert(std::pair<std::string, int32_t>("White", 0xFFFFFFFF));
    }

    void RichText::Update(float delta)
    {
        wavyAngle -= 0.3;
    }

    void RichText::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        int x = renderPosition.x, y = renderPosition.y;

        std::vector<int32_t> colorStack;
        sf::Color formatColor = sf::Color(255, 255, 255);

        bool cancelNext = false;
        float localWavyAngle = wavyAngle;

        for (int i = 0; i < rawText.size(); i++)
        {
            bool verifiedTag = false;

            switch (rawText.at(i))
            {
            case '\n':
                y += (font->GetGlyph('A').texture.height + font->GetGlyph('A').offset + (monospacing == -1 ? 5 : monospacing * 2)) * scale;
                x = renderPosition.x;
                verifiedTag = true;
                break;

            case '\\':
                if (!ignoreTags)
                {
                    if ((size_t)i + 1 < rawText.size())
                    {
                        if (rawText[(size_t)i + 1] == 'i')
                        {
                            y += (font->GetGlyph('A').texture.height + font->GetGlyph('A').offset) * scale;

                            if (monospacing == -1)
                            {
                                x = renderPosition.x + (font->GetGlyph('*').shift + font->GetGlyph(' ').shift) * scale;
                            }
                            else
                            {
                                x = renderPosition.x + (font->GetGlyph('*').texture.width + font->GetGlyph(' ').texture.width + monospacing * 2) * scale;
                            }
                            i += 1;
                            verifiedTag = true;
                            break;
                        }
                    }

                    cancelNext = true;
                    verifiedTag = true;
                }

                break;

            case '[':
                if (!cancelNext && !ignoreTags)
                {
                    std::string temp = rawText.substr((size_t)i + 1, rawText.substr((size_t)i + 1).find_first_of(']'));
                    std::string tempData = temp.substr(temp.find_first_of(":") + 1);
                    bool verifiedTag = false;

                    if (temp[0] == 'c')
                    {
                        if (colorPresets.count(tempData))
                        {
                            formatColor = sf::Color(colorPresets.at(tempData));

                            colorStack.push_back(colorPresets.at(tempData));
                        }
                        else
                        {
                            int32_t hexColor = std::stoul((tempData.length() < 8) ? tempData + "FF" : tempData, 0, 16);

                            formatColor = sf::Color(hexColor);

                            colorStack.push_back(hexColor);
                        }
                    }
                    else if (temp[0] == '/')
                    {
                        if (temp[1] == 'c')
                        {
                            if (colorStack.size() > 1)
                            {
                                colorStack.pop_back();
                                formatColor = sf::Color((colorStack.back()));
                            }
                            else
                            {
                                formatColor = sf::Color(255, 255, 255);
                            }
                        }
                    }

                    i += temp.length() + 2;
                    verifiedTag = true;
                }
                else
                {
                    cancelNext = false;
                }
                break;

            default:
                break;
            }

            if (verifiedTag)
            {
                continue;
            }

            if (i < rawText.length() && font->HasGlyph(rawText.at(i)))
            {
                sf::Vector2f localRenderOffset = {0, 0};
                if (textTypeFlags & static_cast<char>(TextType::Shaky))
                {
                    localRenderOffset.x += (std::rand() % 2 + 1) - (std::rand() % 2 + 1);
                    localRenderOffset.y += (std::rand() % 2 + 1) - (std::rand() % 2 + 1);
                }
                if (textTypeFlags & static_cast<char>(TextType::Wavy))
                {
                    localRenderOffset.x += (std::cos(localWavyAngle) * 0.75);
                    localRenderOffset.y += (std::sin(localWavyAngle) * 1.75);
                }

                auto glyph = font->GetGlyph(rawText.at(i));
                auto sprite = font->GetGlyphSprite(rawText.at(i));
                sprite.setPosition(x + localRenderOffset.x, y + localRenderOffset.y);
                sprite.setScale({scale, scale});
                sprite.color = formatColor;

                if (monospacing == -1)
                {
                    x += (glyph.shift) * scale;
                }
                else
                {
                    x += (glyph.texture.width + monospacing) * scale;
                }

                localWavyAngle--;

                target.draw(sprite, states);
            }
        }
    }
}