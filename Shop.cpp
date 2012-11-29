#include "Shop.h"
#include "Inventory.h"
#include "Client.h"
#include "Graphics.h"

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

	if(GetClient()->IsLocalPlayerSelected())
		pGraphics->DrawScreenQuad(nullptr, 700, 770, 20, 20);
}

void Shop::OnHoover(const ItemSlot& itemSlot)
{

}

void Shop::OnLeftPress(const ItemSlot& itemSlot)
{
	
}

void Shop::OnRightPress(const ItemSlot& itemSlot)
{
	// If the Clients Player isn't selected then select it before adding item to inventory.
	if(!GetClient()->IsLocalPlayerSelected())
		GetClient()->SetSelectedPlayer(GetClient()->GetPlayer());

	if(mInspectingInventory != nullptr && itemSlot.taken)
		mInspectingInventory->AddItem(itemSlot.item);
}

void Shop::SetInspectingInventory(Inventory* pIventory)
{
	mInspectingInventory = pIventory;
}

void Shop::SetClientsPlayerI(int id)
{

}