#include "RenderSprite.h"
#include "BatchHandler.h"

namespace SBURB
{
    RenderSprite::RenderSprite()
    {
        textureId = -1;
        textureRect = sf::IntRect(0, 0, 0, 0);
        color = sf::Color::White;
    }

    RenderSprite::RenderSprite(int texId, sf::IntRect texRect)
    {
        textureId = texId;
        textureRect = texRect;
        color = sf::Color::White;
    }

    void RenderSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        const auto& rect = textureRect;
        sf::FloatRect transformRect({ 0, 0 }, sf::Vector2f(rect.width, rect.height));
        transformRect = states.transform.transformRect(transformRect);
        sf::VertexArray arr(sf::Quads, 4);
        arr[0].position = sf::Vector2f(transformRect.left, transformRect.top);
        arr[1].position = sf::Vector2f(transformRect.left + transformRect.width, transformRect.top);
        arr[2].position = sf::Vector2f(transformRect.left + transformRect.width, transformRect.top + transformRect.height);
        arr[3].position = sf::Vector2f(transformRect.left, transformRect.top + transformRect.height);

        arr[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
        arr[1].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top);
        arr[2].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height);
        arr[3].texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height);

        arr[0].color = color;
        arr[1].color = color;
        arr[2].color = color;
        arr[3].color = color;

        BatchHandler::getInstance().DrawSpriteRect(textureId, arr, target);
    }
}