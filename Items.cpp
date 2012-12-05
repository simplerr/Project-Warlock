#include "Items.h"

//
// BaseItem
//

BaseItem::BaseItem(string iconName) 
{
	SetCost(4);
	SetSkill(false);
	SetLevel(1);
	mIconTexture = GLib::GetGraphics()->LoadTexture(iconName);
}

BaseItem::~BaseItem() 
{

}

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

void BaseItem::SetAttributes(Attributes attributes)
{
	mAttributes = attributes;
}

bool BaseItem::IsSkill()
{
	return mSkill;
}

void BaseItem::SetName(ItemName name)
{
	mName = name;
}

void BaseItem::SetDescription(string description)
{
	mDescription = description;
}

Attributes	BaseItem::GetAttributes()
{
	return mAttributes;
}

string BaseItem::GetDescription()
{
	return mDescription;
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

//
// Item
//

Item::Item(string iconName) 
	: BaseItem(iconName)
{
	
}

Item::~Item() 
{

}

void Item::DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size) 
{
	pGraphics->DrawScreenQuad(GetIconTexture(), pos.x, pos.y, size, size);
}