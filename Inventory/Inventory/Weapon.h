#pragma once
#include "Item.h"

class Weapon
	: public Item
{
public:
	Weapon();
	~Weapon();

public:
};

using WeaponPtr = unique_ptr<Weapon>;
