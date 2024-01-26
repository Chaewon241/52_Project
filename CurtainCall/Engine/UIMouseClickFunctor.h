#pragma once
#include "UIFunctor.h"

class UIMouseClickFunctor :
    public UIFunctor
{
public:
   virtual void Execute(const UIObject& onwer, float deltaTime) override;
};