#include "Room.h"

namespace SBURB
{
    Room::Room()
        : size({0, 0})
    {
    }

    Room::Room(sf::Vector2f size)
        : size(size)
    {
    }

    void Room::Initialize()
    {
        if (isInitialized)
            return;
        for (auto &object : objects)
        {
            object->Init();
        }

        for (auto &zone : zones)
        {
            zone->Init();
        }

        isInitialized = true;
    }
}
