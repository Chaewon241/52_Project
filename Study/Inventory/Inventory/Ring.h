#pragma once
#include "Item.h"

class Ring :
    public Item
{
public:
    Ring();
    ~Ring();
};

using RingPtr = unique_ptr<Ring>;