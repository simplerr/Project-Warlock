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

SkillInventory::SkillInventory(int x, int y, int colums, float slotSize)
	: ItemContainer(x, y, colums, slotSize)
{
	mPlayer = nullptr;

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
	if(!HasFreeSlots())
		return;

	Item* item = (Item*)pItem;

	ItemName name = GetItemLoader()->StringToName(item->name);
	int level = item->GetLevel();

	Skill* skill = mPlayer->AddSkill(SKILL_FIREBALL);
	skill->SetCost(pItem->GetCost());

	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(mPlayer->GetId());
	bitstream.Write(name);
	bitstream.Write(level);
	GetClient()->SendServerMessage(bitstream);

	UpdateItems();
}

void SkillInventory::RemoveSkill(ItemName type)
{
	mPlayer->RemoveSkill(type);

	//// Send to server.
	//RakNet::BitStream bitstream;
	//bitstream.Write((unsigned char)NMSG_ITEM_REMOVED);
	//bitstream.Write(mPlayer->GetId());
	//bitstream.Write(name);
	//bitstream.Write(level);
	//GetClient()->SendServerMessage(bitstream);

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

void SkillInventory::OnRightPress(const ItemSlot& itemSlot)
{
	// Don't do anything unless the local player is selected.
	if(!GetClient()->IsLocalPlayerSelected() || GetClient()->GetArenaState() == PLAYING_STATE)
		return;

	mPlayer->RemoveSkill(((Skill*)itemSlot.item)->GetName());

	// Sell item.
	mPlayer->RemoveSkill(itemSlot.item->GetName());

	//// Send to server.
	RakNet::BitStream bitstream1;
	bitstream1.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream1.Write(mPlayer->GetId());
	bitstream1.Write(itemSlot.item->GetName());
	bitstream1.Write(itemSlot.item->GetLevel());
	GetClient()->SendServerMessage(bitstream1);

	UpdateItems();


	Player* player = GetClient()->GetPlayer();
	player->SetGold(player->GetGold() + itemSlot.item->GetCost() - 3); // [NOTE][TODO] Hard coded!!!!

	// Send event to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
	bitstream.Write(player->GetId());
	bitstream.Write(player->GetGold());
	GetClient()->SendServerMessage(bitstream);
}

string SkillInventory::GetHooverText(const BaseItem* pItem)
{
	Skill* item = (Skill*)pItem;

//	char buffer[244];
//	sprintf(buffer, "Sell value: %i gold\n", item->cost - 3);	// [NOTE][TODO] Maybe enough?
//	return string(buffer + item->description);
	return "hoover text";
}

void SkillInventory::SetPlayer(Player* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}