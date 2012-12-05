#include "Shop.h"
#include "NetworkMessages.h"
#include "Inventory.h"
#include "Client.h"
#include "Graphics.h"
#include "Player.h"
#include "BitStream.h"

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

void Shop::Update(GLib::Input* pInput, float dt)
{
	if(GetClient()->GetArenaState() == SHOPPING_STATE)
		ItemContainer::Update(pInput, dt);
}

void Shop::Draw(GLib::Graphics* pGraphics)
{
	if(GetClient()->GetArenaState() == SHOPPING_STATE)
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
	Player* player = GetClient()->GetPlayer();

	// If the Clients Player isn't selected then select it before adding item to inventory.
	if(!GetClient()->IsLocalPlayerSelected())
		GetClient()->SetSelectedPlayer(player);

	Item* item = (Item*)itemSlot.item;

	if(player->GetGold() >= item->GetCost() && mInspectingInventory != nullptr && itemSlot.taken) {
		mInspectingInventory->AddItem(item);
		player->SetGold(player->GetGold() - item->GetCost());

		// Send event to server.
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
		bitstream.Write(player->GetId());
		bitstream.Write(player->GetGold());
		GetClient()->SendServerMessage(bitstream);
	}
}

string Shop::GetHooverText(const BaseItem* pItem)
{
	Item* item = (Item*)pItem;

	char buffer[244];
	sprintf(buffer, "Cost: %i gold\n", item->GetCost());
	return string(buffer + item->description);
}

void Shop::SetInspectingInventory(ItemContainer* pIventory)
{
	mInspectingInventory = pIventory;
}

void Shop::SetClientsPlayerI(int id)
{

}