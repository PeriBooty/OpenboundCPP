#ifndef SBURB_INTERACTABLE_H
#define SBURB_INTERACTABLE_H

#include "Object.h"

namespace SBURB
{
    class Interactable : public Object
    {
    public:
        Interactable();

        virtual void Run(){};
        virtual void Init() override{};
        virtual void Update(float delta) override{};

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override{};

        bool isAction = false;
        bool persists = true;
    };
}
#endif