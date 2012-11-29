#include "Inventory.h"
#include "Client.h"
#include "ItemLoaderXML.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Player.h"
#include "BitStream.h"
#include "NetworkMessages.h"

Inventory::Inventory(int x, int y, int colums, float slotSize)
	: ItemContainer(x, y, colums, slotSize)
{
	mPlayer = nullptr;

	for(int i = 0; i < 6; i++)
		AddSlot();
}

Inventory::~Inventory()
{

}

void Inventory::Update(GLib::Input* pInput, float dt)
{

}

void Inventory::Draw(GLib::Graphics* pGraphics)
{
	ItemContainer::Draw(pGraphics);
}

void Inventory::AddItem(Client* pClient, ItemName name, int level)
{
	mPlayer->AddItem(GetItemLoader(), ItemKey(name, level));

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(name);
	bitstream.Write(level);
	pClient->SendServerMessage(bitstream);

	UpdateItems();
}

void Inventory::RemoveItem(Client* pClient, ItemName name, int level)
{
	mPlayer->RemoveItem(GetItemLoader(), ItemKey(name, level));

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(name);
	bitstream.Write(level);
	pClient->SendServerMessage(bitstream);

	UpdateItems();
}

void Inventory::UpdateItems()
{
	// Get the players items.
	multiset<ItemKey> playerItems = mPlayer->GetItemList();

	FreeAllSlots();

	for(auto iter = playerItems.begin(); iter != playerItems.end(); iter++)
		PlaceInFreeSlot((*iter));
}

void Inventory::OnHoover(const ItemSlot& item)
{

}

void Inventory::OnPress(const ItemSlot& item)
{

}

void Inventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}