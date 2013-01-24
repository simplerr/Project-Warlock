#pragma once
#include "HudItemContainer.h"

class Inventory;
class HudItem;

class Shop : public HudItemContainer
{
public:
	Shop(int x, int y, int colums, float slotSize);
	~Shop();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(ItemSlot& itemSlot);
	string GetHooverText(HudItem* pItem);

	void SetInspectingInventory(HudItemContainer* pIventory);
	void SetClientsPlayerI(int id);

	void AddItem(HudItem* pItem) {}

	void InventoryItemRemoved(HudItem* pItem);
private:
	HudItemContainer* mInspectingInventory;
	int			   mClientPlayerId;
};