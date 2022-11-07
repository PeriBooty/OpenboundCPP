#include "TextWriter.h"
#include <iostream>
namespace SBURB
{
    TextWriter::TextWriter()
    {
        this->font = nullptr;
        this->textPosition = 0;
        this->renderPosition = {0, 0};
        this->richText = RichText();
        this->timeout = 0;
        this->cancelNext = false;
    }

    TextWriter::TextWriter(Font *font)
    {
        this->font = font;
        this->textPosition = 0;
        this->renderPosition = {0, 0};
        this->richText = RichText();
        this->richText.font = font;
        this->timeout = 0;
        this->cancelNext = false;
    }

    void TextWriter::Update(float delta)
    {
        if (timeout <= 0)
        {
            RawDataCheck();
        }
        else
        {
            timeout -= 1;
        }

        richText.Update(delta);
    }

    void TextWriter::RawDataCheck()
    {
        if (rawText.length() > textPosition)
        {
            if (rawText[textPosition] == '\\')
            {
                if (rawText[(size_t)textPosition + 1] == 'n')
                {
                    textPosition += 2;
                    timeout = textSpeed;
                    RawDataCheck();
                }
                else if (rawText[(size_t)textPosition + 1] == 'i')
                {
                    textPosition += 2;
                    timeout = textSpeed;
                    RawDataCheck();
                }
                else
                {
                    cancelNext = true;
                    textPosition++;
                    timeout = textSpeed;
                    RawDataCheck();
                }
            }
            else if (rawText[textPosition] == '[')
            {
                if (!cancelNext)
                {
                    std::string temp = rawText.substr((size_t)textPosition + 1, rawText.substr((size_t)textPosition + 1).find_first_of(']'));
                    bool verifiedTag = false;

                    // Pause
                    switch (temp[0])
                    {
                    case 'p':
                        timeout = std::stoi(temp.substr(temp.find_first_of(":") + 1), nullptr, 0);
                        verifiedTag = true;
                        break;

                    default:
                        timeout = textSpeed;
                        break;
                    }

                    if (verifiedTag)
                    {
                        rawText = rawText.erase(textPosition, temp.length() + 2);
                    }
                    else
                    {
                        textPosition += temp.length() + 2;
                    }
                }
                else
                {
                    cancelNext = false;
                    textPosition++;
                    timeout = textSpeed;
                }
            }
            else
            {
                cancelNext = false;
                textPosition++;
                timeout = textSpeed;
            }
        }

        richText.rawText = rawText.substr(0, textPosition);
    }

    void TextWriter::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(richText, states);
    }
}