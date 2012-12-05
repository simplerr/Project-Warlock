#pragma once
#include <string>
#include "d3dUtil.h"
#include "Graphics.h"

using namespace std;

namespace GLib {
	class Graphics;
	class Texture2D;
}

enum ItemName
{
	IRON_ARMOR,		// Health
	REGEN_CAP,		// Regen
	VIPER_BOOTS,	// Movement speed

	// Skills.
	SKILL_FIREBALL,

};

class BaseItem
{
public:
	BaseItem(string iconName);
	~BaseItem();

	// Draw the icon.
	virtual void Draw(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size) = 0;

	GLib::Texture2D* GetIconTexture();

	void SetCost(int cost);
	void SetSkill(bool skill);
	void SetName(ItemName name);
	void SetLevel(int level);

	int GetLevel();
	ItemName GetName();
	int GetCost();
	bool IsSkill();
private:
	GLib::Texture2D* mIconTexture;
	ItemName mName;
	int		mCost;
	int		mLevel;
	bool	mSkill;
};

class Item : public BaseItem
{
public:
	Item(string iconName);
	~Item();

	void Draw(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size);

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
};

// Inventory has a map of ItemSlots!