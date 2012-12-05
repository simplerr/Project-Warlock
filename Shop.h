#pragma once
#include "ItemContainer.h"

class Inventory;
class BaseItem;

class Shop : public ItemContainer
{
public:
	Shop(int x, int y, int colums, float slotSize);
	~Shop();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(ItemSlot& itemSlot);
	string GetHooverText(BaseItem* pItem);

	void SetInspectingInventory(ItemContainer* pIventory);
	void SetClientsPlayerI(int id);

	void AddItem(BaseItem* pItem) {}

	void InventoryItemRemoved(BaseItem* pItem);
private:
	ItemContainer* mInspectingInventory;
	int			   mClientPlayerId;
};