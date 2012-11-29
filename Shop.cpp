#include "Shop.h"
#include "Inventory.h"

Shop::Shop(int x, int y, int colums, float slotSize)
	: ItemContainer(x, y, colums, slotSize)
{
	SetInspectingInventory(nullptr);

	for(int i = 0; i < 6; i++)
		AddSlot();
}

Shop::~Shop()
{

}

void Shop::Draw(GLib::Graphics* pGraphics)
{
	ItemContainer::Draw(pGraphics);
}

void Shop::OnHoover(const ItemSlot& itemSlot)
{

}

void Shop::OnPress(const ItemSlot& itemSlot)
{
	if(mInspectingInventory != nullptr && itemSlot.taken)
		mInspectingInventory->AddItem(itemSlot.item);
}

void Shop::SetInspectingInventory(Inventory* pIventory)
{
	mInspectingInventory = pIventory;
}