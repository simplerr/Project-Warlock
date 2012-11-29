#pragma once
#include "ItemContainer.h"

class Inventory;

class Shop : public ItemContainer
{
public:
	Shop(int x, int y, int colums, float slotSize);
	~Shop();

	void Draw(GLib::Graphics* pGraphics);
	void OnHoover(const ItemSlot& itemSlot);
	void OnPress(const ItemSlot& itemSlot);

	void SetInspectingInventory(Inventory* pIventory);
private:
	Inventory* mInspectingInventory;
};