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
	ItemContainer::Update(pInput, dt);
}

void Inventory::Draw(GLib::Graphics* pGraphics)
{
	ItemContainer::Draw(pGraphics);

	if(GetClient()->IsLocalPlayerSelected())
		pGraphics->DrawScreenQuad(nullptr, 760, 770, 20, 20);
}

void Inventory::AddItem(ItemName name, int level)
{
	AddItem(GetItemLoader()->GetItem(ItemKey(name, level)));
}

void Inventory::AddItem(Item item)
{
	if(!HasFreeSlots())
		return;

	ItemName name = GetItemLoader()->StringToName(item.name);
	int level = item.level;

	mPlayer->AddItem(GetItemLoader(), ItemKey(name, level));

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(name);
	bitstream.Write(level);
	GetClient()->SendServerMessage(bitstream);

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
	GetClient()->SendServerMessage(bitstream);

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

void Inventory::OnLeftPress(const ItemSlot& itemSlot)
{

}

void Inventory::OnRightPress(const ItemSlot& itemSlot)
{
	// Don't do anything unless the local player is selected.
	if(!GetClient()->IsLocalPlayerSelected())
		return;

	// Sell item.
	RemoveItem(GetItemLoader()->StringToName(itemSlot.item.name), itemSlot.item.level);
	Player* player = GetClient()->GetPlayer();
	player->SetGold(player->GetGold() + itemSlot.item.cost - 3); // [NOTE][TODO] Hard coded!!!!

	// Send event to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
	bitstream.Write(player->GetId());
	bitstream.Write(player->GetGold());
	GetClient()->SendServerMessage(bitstream);
}

string Inventory::GetHooverText(const Item& item)
{
	char buffer[244];
	sprintf(buffer, "Sell value: %i gold\n", item.cost - 3);	// [NOTE][TODO] Maybe enough?
	return string(buffer + item.description);
}

void Inventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}