#include "TileMap.h"

namespace SBURB
{
    TileMap::TileMap(int texId, const TileGrid &grid, const TileSet &tileset)
        : textureId(texId)
    {
        InitSystem(grid, tileset);
        this->grid = grid;
        this->tileset = tileset;
    }

    void TileMap::InitSystem(const TileGrid &grid, const TileSet &tileset)
    {
        if (grid.size() == 0 || tileset.size() == 0)
            throw EmptyTileException();
        for (int y = 0; y < grid.size(); ++y)
        {
            for (int x = 0; x < grid[y].size(); ++x)
            {
                if (tileset.find(grid[y][x]) == tileset.end())
                    throw InvalidTileException();
                if (tileWidth == -1 || tileHeight == -1)
                {
                    tileWidth = tileset.at(grid[y][x]).width;
                    tileHeight = tileset.at(grid[y][x]).height;
                }
                else
                {
                    if (tileset.at(grid[y][x]).width != tileWidth || tileset.at(grid[y][x]).height != tileHeight)
                    {
                        throw InvalidTileException();
                    }
                }
            }
        }
    }

    int TileMap::GetWidth() const
    {
        int width = 0;
        for (auto &t : grid)
        {
            if (t.size() > width)
                width = (int)t.size();
        }
        return width;
    }

    int TileMap::GetHeight() const
    {
        return (int)grid.size();
    }

    int TileMap::GetRenderWidth() const
    {
        int renderWidth = 0;
        for (auto &tv : grid)
        {
            int temp = 0;
            for (auto t : tv)
            {
                temp += tileset.at(t).width;
            }
            if (temp > renderWidth)
                renderWidth = temp;
        }
        return renderWidth;
    }

    int TileMap::GetRenderHeight() const
    {
        int renderHeight = 0;
        for (auto &tv : grid)
        {
            int temp = 0;
            for (auto &t : tv)
            {
                temp += tileset.at(t).height;
            }
            if (temp > renderHeight)
                renderHeight = temp;
        }
        return renderHeight;
    }

    int TileMap::GetTileWidth() const
    {
        return tileWidth;
    }

    int TileMap::GetTileHeight() const
    {
        return tileHeight;
    }

    void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        int drawx = 0, drawy = 0;
        for (auto &tv : grid)
        {
            for (auto &t : tv)
            {
                auto sprite = Sprite(textureId, tileset.at(t));
                sprite.setPosition((float)drawx, (float)drawy);
                target.draw(sprite, states);
                drawx += tileWidth;
            }
            drawx = 0;
            drawy += tileHeight;
        }
    }
}