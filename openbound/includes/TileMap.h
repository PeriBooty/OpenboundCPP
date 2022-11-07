#ifndef SBURB_TILE_MAP_H
#define SBURB_TILE_MAP_H

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Sprite.h"

namespace SBURB
{
    using TileGrid = std::vector<std::vector<unsigned short>>;
    using TileSet = std::unordered_map<unsigned short, sf::IntRect>;

    class EmptyTileException : public std::exception
    {
    public:
        explicit EmptyTileException() {}

        virtual const char *what() const noexcept
        {
            return "TileGrid or TileSet was empty.";
        }
    };

    class InvalidTileException : public std::exception
    {
    public:
        explicit InvalidTileException() {}

        virtual const char *what() const noexcept
        {
            return "A Tile in the TileGrid didn't match the tile in the TileSet.";
        }
    };

    class TileMap : public Object
    {
    public:
        TileMap() : grid(), tileset() {}
        TileMap(int texId, const TileGrid &grid, const TileSet &tileset);

        void InitSystem(const TileGrid &grid, const TileSet &tileset);

        int GetWidth() const;
        int GetHeight() const;
        int GetRenderWidth() const;
        int GetRenderHeight() const;
        int GetTileWidth() const;
        int GetTileHeight() const;

    protected:
        TileGrid grid;
        TileSet tileset;

        int textureId = -1;

    private:
        int tileWidth = -1;
        int tileHeight = -1;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };
}

#endif