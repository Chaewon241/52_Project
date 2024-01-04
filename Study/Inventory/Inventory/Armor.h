#pragma once
#include "Item.h"

class Armor :
    public Item
{
public:
    Armor();
    ~Armor();
};

using ArmorPtr = unique_ptr<Armor>;