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
	mClient = nullptr;

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

void Inventory::AddItem(ItemName name, int level)
{
	AddItem(GetItemLoader()->GetItem(ItemKey(name, level)));
}

void Inventory::AddItem(Item item)
{
	ItemName name = GetItemLoader()->StringToName(item.name);
	int level = item.level;

	mPlayer->AddItem(GetItemLoader(), ItemKey(name, level));

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(name);
	bitstream.Write(level);
	mClient->SendServerMessage(bitstream);

	UpdateItems();
}

void Inventory::RemoveItem(ItemName name, int level)
{
	mPlayer->RemoveItem(GetItemLoader(), ItemKey(name, level));

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(name);
	bitstream.Write(level);
	mClient->SendServerMessage(bitstream);

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

void Inventory::OnHoover(const ItemSlot& itemSlot)
{

}

void Inventory::OnPress(const ItemSlot& itemSlot)
{

}

void Inventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}

void Inventory::SetClient(Client* pClient)
{
	mClient = pClient;
}