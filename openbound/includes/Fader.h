#ifndef SBURB_FADER_H
#define SBURB_FADER_H

#include "Object.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <chrono>

namespace SBURB
{
    class Fader : public Object
    {
    public:
        virtual void Init() override;
        virtual void Update(float delta) override;

        static void Update();

        static void Setup(sf::IntRect rectSize = {0, 0, 640, 480}, int fadeTime = 1);

        static bool IsDone();
        static bool IsReversing();

        static void Reverse();

        static Fader &GetInstance() { return instance; }

    private:
        Fader() = default;
        static Fader instance;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        sf::RectangleShape fadeRect;
        std::chrono::high_resolution_clock::time_point start;

        sf::IntRect rectSize;
        int msCurrTime;
        int msRunTime;
        bool done = false;
        bool reverse = false;
    };
}

#endif
