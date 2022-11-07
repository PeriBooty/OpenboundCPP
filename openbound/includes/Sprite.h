#ifndef SBURB_SPRITE_H
#define SBURB_SPRITE_H
#include <SFML/Graphics.hpp>

#include "Common.h"
#include "Object.h"
#include "Texture.h"

namespace SBURB
{
    class Sprite : public sf::Drawable, public sf::Transformable
    {
    public:
        Sprite();
        Sprite(int texId, sf::IntRect texRect);

        sf::IntRect textureRect;
        sf::Color color;
        int textureId;

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };
}

#endif