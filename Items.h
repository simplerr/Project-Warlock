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
	//
	// Items
	//
	IRON_ARMOR,		// Health
	REGEN_CAP,		// Regen
	VIPER_BOOTS,	// Movement speed

	//
	// Skills
	//
	SKILL_FIREBALL,	// Fireball

};

struct Attributes
{
	Attributes() {
		regen = knockbackResistance = lavaImmunity = damage = lifesteal = health = cooldown = 0.0f;
	}

	float health;
	float regen;
	float movementSpeed;
	float knockbackResistance;
	float lavaImmunity;
	float damage;
	float lifesteal;
	float cooldown;
};

class BaseItem
{
public:
	BaseItem(string iconName);
	~BaseItem();

	// Draw the icon.
	virtual void DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size) = 0;

	void SetCost(int cost);
	void SetSkill(bool skill);
	void SetName(ItemName name);
	void SetLevel(int level);
	void SetAttributes(Attributes attributes);
	void SetDescription(string description);

	GLib::Texture2D* GetIconTexture();
	ItemName		 GetName();
	Attributes		 GetAttributes();
	string			 GetDescription();
	int				 GetLevel();
	int				 GetCost();
	bool			 IsSkill();
private:
	GLib::Texture2D*	mIconTexture;
	ItemName			mName;
	Attributes			mAttributes;
	string				mDescription;
	int		 mCost;
	int		 mLevel;
	bool	 mSkill;
};

class Item : public BaseItem
{
public:
	Item(string iconName);
	~Item();

	void DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size);
};

// Inventory has a map of ItemSlots!