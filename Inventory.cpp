#include "Inventory.h"
#include "Client.h"
#include "ItemLoaderXML.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Player.h"
#include "BitStream.h"
#include "NetworkMessages.h"

Inventory::Inventory()
{
	mPosition = XMFLOAT2(970, 770);
	mIconSize = 60.0f;
	mPlayer = nullptr;

	for(int i = 0; i < 6; i++)
		mInventorySlots.push_back(InventorySlot());

	mEmptySlotTexture = GLib::GetGraphics()->LoadTexture("textures/icons/empty_slot.bmp");
}

Inventory::~Inventory()
{

}

void Inventory::Update(GLib::Input* pInput, float dt)
{

}

void Inventory::Draw(GLib::Graphics* pGraphics)
{
	for(int i = 0; i < mInventorySlots.size(); i++)
	{
		XMFLOAT2 pos = mPosition;

		pos.x += mIconSize * 1.2 * (i % 3);
		pos.y += mIconSize * 1.2 * (i / 3);

		if(mInventorySlots[i].taken) 
			pGraphics->DrawScreenQuad(mInventorySlots[i].texture, pos.x, pos.y, mIconSize, mIconSize);
		else
			pGraphics->DrawScreenQuad(mEmptySlotTexture, pos.x, pos.y, mIconSize, mIconSize);
	}
}

void Inventory::AddItem(Client* pClient, ItemName name, int level)
{
	mPlayer->AddItem(mItemLoaderXML, ItemKey(name, level));

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
	mPlayer->RemoveItem(mItemLoaderXML, ItemKey(name, level));

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
	// Update the inventory.
	multiset<ItemKey> playerItems = mPlayer->GetItemList();

	for(int i = 0; i< mInventorySlots.size(); i++)
		mInventorySlots[i].taken = false;

	for(auto iter = playerItems.begin(); iter != playerItems.end(); iter++)
		PlaceInFreeSlot((*iter));
}

void Inventory::PlaceInFreeSlot(ItemKey itemKey)
{
	Item item = mItemLoaderXML->GetItem(itemKey);
	for(int i = 0; i < mInventorySlots.size(); i++)
	{
		if(!mInventorySlots[i].taken)
		{
			mInventorySlots[i].item = item;
			mInventorySlots[i].texture = GLib::GetGraphics()->LoadTexture(item.icon);
			mInventorySlots[i].taken = true;
			break;
		}
	}
}

void Inventory::SetItemLoader(ItemLoaderXML* pLoader)
{
	mItemLoaderXML = pLoader;
}

void Inventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}