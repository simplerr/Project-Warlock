#pragma once

#include <string>
using namespace std;

enum ItemName
{
	IRON_ARMOR,		// Health
	REGEN_CAP,		// Regen
	VIPER_BOOTS		// Movement speed
};

struct Item
{
	Item() {
		health =  regen = movementSpeed = knockbakResistance = damage = lifesteal = 0.0f;
		price = 10;
	}

	string description;
	string icon;
	string name;
	float health;
	float regen;
	float movementSpeed;
	float knockbakResistance;
	float lavaImmunity;
	float damage;
	float lifesteal;
	int level;
	int price;
};

// Inventory has a map of ItemSlots!