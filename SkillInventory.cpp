#include "SkillInventory.h"

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

SkillInventory::SkillInventory(int x, int y, int colums, float slotSize)
	: ItemContainer(x, y, colums, slotSize)
{
	mPlayer = nullptr;
	mShop = nullptr;

	for(int i = 0; i < 8; i++)
		AddSlot();
}

SkillInventory::~SkillInventory()
{

}

void SkillInventory::Update(GLib::Input* pInput, float dt)
{
	ItemContainer::Update(pInput, dt);
}

void SkillInventory::Draw(GLib::Graphics* pGraphics)
{
	ItemContainer::Draw(pGraphics);

	if(GetClient()->IsLocalPlayerSelected())
		pGraphics->DrawScreenQuad(nullptr, 760, 770, 20, 20);
}

void SkillInventory::AddItem(ItemName name, int level)
{
	AddItem(GetItemLoader()->GetItem(ItemKey(name, level)));
}

void SkillInventory::AddItem(BaseItem* pItem)
{
	// Any free slots?
	if(!HasFreeSlots())
		return;

	Skill* skill = mPlayer->AddSkill(pItem->GetName());
	
	// Pass on the attributes.
	// [NOTE] REALLY UGLY [HACK][WARNING].
	skill->SetAttributes(pItem->GetAttributes());
	skill->SetDescription(pItem->GetDescription());
	skill->SetCost(pItem->GetCost());
	skill->SetLevel(pItem->GetLevel());

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(pItem->GetName());
	bitstream.Write(pItem->GetLevel());
	GetClient()->SendServerMessage(bitstream);

	UpdateItems();
}

void SkillInventory::RemoveSkill(BaseItem* pItem)
{
	// Sell item.
	mPlayer->RemoveSkill(pItem->GetName());

	// Send to server.
	RakNet::BitStream bitstream1;
	bitstream1.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream1.Write(mPlayer->GetId());
	bitstream1.Write(pItem->GetName());
	bitstream1.Write(pItem->GetLevel());
	GetClient()->SendServerMessage(bitstream1);

	mShop->InventoryItemRemoved(pItem);

	UpdateItems();
}

void SkillInventory::UpdateItems()
{
	// Get the players items.
	std::map<int, Skill*> skillMap = mPlayer->GetSkillMap();

	FreeAllSlots();

	for(auto iter = skillMap.begin(); iter != skillMap.end(); iter++)
		PlaceInFreeSlot((*iter).second);
}

void SkillInventory::OnHoover(const ItemSlot& itemSlot)
{

}

void SkillInventory::OnLeftPress(const ItemSlot& itemSlot)
{

}

void SkillInventory::OnRightPress(ItemSlot& itemSlot)
{
	// Don't do anything unless the local player is selected.
	if(!GetClient()->IsLocalPlayerSelected() || GetClient()->GetArenaState() == PLAYING_STATE)
		return;

	// Sell skill.
	RemoveSkill(itemSlot.item);
	Player* player = GetClient()->GetPlayer();
	player->SetGold(player->GetGold() + itemSlot.item->GetCost() - 3); // [NOTE][TODO] Hard coded!!!!

	// Send event to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
	bitstream.Write(player->GetId());
	bitstream.Write(player->GetGold());
	GetClient()->SendServerMessage(bitstream);
}

string SkillInventory::GetHooverText(BaseItem* pItem)
{
	char buffer[244];
	sprintf(buffer, "Sell value: %i gold\nLevel: %i", pItem->GetCost() - 3, pItem->GetLevel());	// [NOTE][TODO] Maybe enough?
	return string(buffer + pItem->GetDescription());
}

void SkillInventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}

void SkillInventory::SetShop(Shop* pShop)
{
	mShop = pShop;
}