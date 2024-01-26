#pragma once
#include "UIFunctor.h"

class UIMouseHoverFunctor :
    public UIFunctor
{
public:
    virtual void Execute(const UIObject& onwer, float deltaTime) override;
};