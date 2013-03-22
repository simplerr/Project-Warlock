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
	BOOTS_OF_SPEED,	// Movement speed
	MASK_OF_MADNESS,
	KNOCKBACK_SHIELD,
	LAVA_STAFF,
	MAGIC_WAND,

	//
	// Skills
	//
	SKILL_FIREBALL,	// Fireball
	SKILL_FROSTNOVA,
	SKILL_TELEPORT,
	SKILL_METEOR,
	SKILL_HOOK,
	SKILL_VENOM,
	SKILL_GRAPPLING_HOOK,
	
	//
	// Status Effects
	//
	FREEZE_STATUS,
	TELEPORT_STATUS,
	POISON_STATUS
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
	float slow;
	float dot;	// damage per second
	float duration;
	string name;
	string hotkey;
};

/************************************************************************
 This is the steps required for adding new items

 1.) Add a new node to items.xml
 2.) Add new item name to ItemLoaderXML::SetupKeyMap()
		mStringKeyMap["FREEZE_EFFECT"] = FREEZE_STATUS
 
 And for skills you also have to do this

 3.) Add a new skill class connected with the skill name
 4.) Let SkillHandler::AddSkill(ItemName skillName) handle the new skill type
		if(skillName == SKILL_FIREBALL) 
		{
			skill = new FireBall("textures/icons/fireball.png");
			key = 'Q';
		}
 5.) Let ServerSkillInterpreter::Interpret handle the new skill type as well
 6.) Let ClientSkillInterpreter::Interpret handle the new skill type as well
 7.) ???
 8.) Profit!

/************************************************************************/

/**
	Base class for all items, skills and powerups
*/
class HudItem
{
public:
	HudItem(string iconName);
	~HudItem();

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

class Item : public HudItem
{
public:
	Item(string iconName);
	~Item();

	void DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size);
};

// Inventory has a map of ItemSlots!