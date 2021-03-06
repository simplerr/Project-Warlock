#include "Inventory.h"
#include "Client.h"
#include "ItemLoaderXML.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Player.h"
#include "BitStream.h"
#include "NetworkMessages.h"
#include "Skills.h"
#include "Shop.h"

Inventory::Inventory(int x, int y, int colums, float slotSize)
	: HudItemContainer(x, y, colums, slotSize)
{
	SetHooverInfoPos(XMFLOAT2(-slotSize/3, -110));
	SetHooverBkgd("data/textures/white_transparent.png");

	mPlayer = nullptr;
	mShop = nullptr;

	// Add 6 slots.
	for(int i = 0; i < 6; i++)
		AddSlot();
}

Inventory::~Inventory()
{

}

void Inventory::Update(GLib::Input* pInput, float dt)
{
	HudItemContainer::Update(pInput, dt);
}

void Inventory::Draw(GLib::Graphics* pGraphics)
{
	HudItemContainer::Draw(pGraphics);
}

void Inventory::AddItem(ItemName name, int level)
{
	AddItem(GetItemLoader()->GetItem(ItemKey(name, level)));
}

void Inventory::AddItem(HudItem* pItem)
{
	// Any free slots?
	if(!HasFreeSlots())
		return;

	// Remove previous level item.
	if(pItem->GetLevel() > 1) 
		RemoveItem(GetItemLoader()->GetItem(ItemKey(pItem->GetName(), pItem->GetLevel()-1)));

	// Add item to player.
	mPlayer->AddItem(pItem);

	// Send message to server.
	SendItemAdded(mPlayer->GetId(), pItem->GetName(), pItem->GetLevel());

	// Clear the inventory and update it with the players current items.
	UpdateItems();
}

void Inventory::RemoveItem(HudItem* pItem, bool updateLayout)
{
	// Remove item from player.
	mPlayer->RemoveItem(pItem);

	// Send message to server.
	SendItemRemoved(mPlayer->GetId(), pItem->GetName(), pItem->GetLevel());

	// Clear the inventory and update it with the players current items.
	if(updateLayout)
		UpdateItems();

	// Tell the shop.
	mShop->InventoryItemRemoved(pItem);
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

void Inventory::OnRightPress(ItemSlot& itemSlot)
{
	// Don't do anything unless the local player is selected.
	if(!GetClient()->IsLocalPlayerSelected() || GetClient()->GetArenaState() == PLAYING_STATE)
		return;

	// Sell item.
	RemoveItem(itemSlot.item);
	Player* player = GetClient()->GetLocalPlayer();
	player->SetGold(player->GetGold() + itemSlot.item->GetCost() - 1); // [NOTE][TODO] Hard coded!!!!

	// Send message to server.
	SendGoldChange(player->GetId(), player->GetGold());
}

string Inventory::GetHooverText(HudItem* pItem)
{
	char buffer[244];
	sprintf(buffer, "Sell value: %i gold\nLevel: %i\n", pItem->GetCost() - 1, pItem->GetLevel());
	return string(buffer + pItem->GetDescription());
}

void Inventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}

void Inventory::SetShop(Shop* pShop)
{
	mShop = pShop;
}