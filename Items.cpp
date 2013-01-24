#include "Items.h"

//
// BaseItem
//

HudItem::HudItem(string iconName) 
{
	SetCost(4);
	SetSkill(false);
	SetLevel(1);
	mIconTexture = GLib::GetGraphics()->LoadTexture(iconName);
}

HudItem::~HudItem() 
{

}

GLib::Texture2D* HudItem::GetIconTexture() 
{
	return mIconTexture;
}

void HudItem::SetCost(int cost) 
{
	mCost = cost;
}

int HudItem::GetCost() 
{
	return mCost;
}

void HudItem::SetSkill(bool skill)
{
	mSkill = skill;
}

void HudItem::SetAttributes(Attributes attributes)
{
	mAttributes = attributes;
}

bool HudItem::IsSkill()
{
	return mSkill;
}

void HudItem::SetName(ItemName name)
{
	mName = name;
}

void HudItem::SetDescription(string description)
{
	mDescription = description;
}

Attributes	HudItem::GetAttributes()
{
	return mAttributes;
}

string HudItem::GetDescription()
{
	return mDescription;
}

ItemName HudItem::GetName()
{
	return mName;
}

void HudItem::SetLevel(int level)
{
	mLevel = level;
}

int HudItem::GetLevel()
{
	return mLevel;
}

//
// Item
//

Item::Item(string iconName) 
	: HudItem(iconName)
{
	
}

Item::~Item() 
{

}

void Item::DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size) 
{
	pGraphics->DrawScreenQuad(GetIconTexture(), pos.x, pos.y, size, size);
}