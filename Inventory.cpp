#include "Inventory.h"
#include "Client.h"
#include "ItemLoaderXML.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Player.h"
#include "BitStream.h"
#include "NetworkMessages.h"
#include "Skills.h"

Inventory::Inventory(int x, int y, int colums, float slotSize)
	: ItemContainer(x, y, colums, slotSize)
{
	mPlayer = nullptr;

	// Add 6 slots.
	for(int i = 0; i < 6; i++)
		AddSlot();
}

Inventory::~Inventory()
{

}

void Inventory::Update(GLib::Input* pInput, float dt)
{
	ItemContainer::Update(pInput, dt);
}

void Inventory::Draw(GLib::Graphics* pGraphics)
{
	ItemContainer::Draw(pGraphics);
}

void Inventory::AddItem(ItemName name, int level)
{
	AddItem(GetItemLoader()->GetItem(ItemKey(name, level)));
}

void Inventory::AddItem(BaseItem* pItem)
{
	// Any free slots?
	if(!HasFreeSlots())
		return;

	// Add item to player.
	mPlayer->AddItem(pItem);

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(pItem->GetName());
	bitstream.Write(pItem->GetLevel());
	GetClient()->SendServerMessage(bitstream);

	// Clear the inventory and update it with the players current items.
	UpdateItems();
}

void Inventory::RemoveItem(BaseItem* pItem)
{
	// Remove item from player.
	mPlayer->RemoveItem(pItem);

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(pItem->GetName());
	bitstream.Write(pItem->GetLevel());
	GetClient()->SendServerMessage(bitstream);

	// Clear the inventory and update it with the players current items.
	UpdateItems();
}

void Inventory::UpdateItems()
{
	// Get the players items.
	multiset<ItemKey> playerItems = mPlayer->GetItemList();

	// Free all slots.
	FreeAllSlots();

	// Add all player items.
	for(auto iter = playerItems.begin(); iter != playerItems.end(); iter++)
		PlaceInFreeSlot((*iter));
}

void Inventory::OnHoover(const ItemSlot& itemSlot)
{

}

void Inventory::OnLeftPress(const ItemSlot& itemSlot)
{

}

void Inventory::OnRightPress(const ItemSlot& itemSlot)
{
	// Don't do anything unless the local player is selected.
	if(!GetClient()->IsLocalPlayerSelected() || GetClient()->GetArenaState() == PLAYING_STATE)
		return;

	// Sell item.
	RemoveItem(itemSlot.item);
	Player* player = GetClient()->GetPlayer();
	player->SetGold(player->GetGold() + itemSlot.item->GetCost() - 3); // [NOTE][TODO] Hard coded!!!!

	// Send event to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
	bitstream.Write(player->GetId());
	bitstream.Write(player->GetGold());
	GetClient()->SendServerMessage(bitstream);
}

string Inventory::GetHooverText(BaseItem* pItem)
{
	char buffer[244];
	sprintf(buffer, "Sell value: %i gold\n", pItem->GetCost() - 3);	// [NOTE][TODO] Maybe enough?
	return string(buffer + pItem->GetDescription());
}

void Inventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}