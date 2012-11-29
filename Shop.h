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
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(const ItemSlot& itemSlot);

	void SetInspectingInventory(Inventory* pIventory);
	void SetClientsPlayerI(int id);
private:
	Inventory* mInspectingInventory;
	int mClientPlayerId;
};