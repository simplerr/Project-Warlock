#pragma once
#include "ItemContainer.h"
#include "Skills.h"

namespace GLib {
	class Graphics;
	class Input;
	class Texture2D;
}

class Player;
class PlayerModule;
class Shop;

class SkillInventory : public ItemContainer
{
public:
	SkillInventory(int x, int y, int colums, float slotSize);
	~SkillInventory();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void AddItem(ItemName name, int level);
	void AddItem(BaseItem* pItem);
	void RemoveSkill(BaseItem* pItem);
	void UpdateItems();

	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(ItemSlot& itemSlot);
	string GetHooverText(BaseItem* pItem);

	void SetPlayer(PlayerModule* pPlayer);
	void SetShop(Shop* pShop);
private:
	PlayerModule*	mPlayer;
	Shop*			mShop;	 
};