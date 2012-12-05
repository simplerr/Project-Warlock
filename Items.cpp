#include "Items.h"

BaseItem::BaseItem(string iconName) 
{
	SetCost(4);
	SetSkill(false);
	SetLevel(1);
	mIconTexture = GLib::GetGraphics()->LoadTexture(iconName);
};

BaseItem::~BaseItem() 
{

};

GLib::Texture2D* BaseItem::GetIconTexture() 
{
	return mIconTexture;
}

void BaseItem::SetCost(int cost) 
{
	mCost = cost;
}

int BaseItem::GetCost() 
{
	return mCost;
}

void BaseItem::SetSkill(bool skill)
{
	mSkill = skill;
}

bool BaseItem::IsSkill()
{
	return mSkill;
}

void BaseItem::SetName(ItemName name)
{
	mName = name;
}

ItemName BaseItem::GetName()
{
	return mName;
}

void BaseItem::SetLevel(int level)
{
	mLevel = level;
}

int BaseItem::GetLevel()
{
	return mLevel;
}


Item::Item(string iconName) 
	: BaseItem(iconName)
{
	health =  regen = movementSpeed = knockbakResistance = damage = lifesteal = 0.0f;
}

Item::~Item() 
{

};

void Item::Draw(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size) 
{
	pGraphics->DrawScreenQuad(GetIconTexture(), pos.x, pos.y, size, size);
}