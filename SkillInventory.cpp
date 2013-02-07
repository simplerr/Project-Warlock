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
#include "PlayerModule.h"

SkillInventory::SkillInventory(int x, int y, int colums, float slotSize)
	: HudItemContainer(x, y, colums, slotSize)
{
	SetHooverInfoPos(XMFLOAT2(-slotSize/2, -80));
	SetHooverBkgd("textures/white_transparent.png");

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
	HudItemContainer::Update(pInput, dt);
}

void SkillInventory::Draw(GLib::Graphics* pGraphics)
{
	HudItemContainer::Draw(pGraphics);
}

void SkillInventory::AddItem(ItemName name, int level)
{
	AddItem(GetItemLoader()->GetItem(ItemKey(name, level)));
}

void SkillInventory::AddItem(HudItem* pItem)
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

	// Send message to server.
	SendItemAdded(mPlayer->GetPlayer()->GetId(), pItem->GetName(), pItem->GetLevel());

	UpdateItems();
}

void SkillInventory::RemoveSkill(HudItem* pItem)
{
	// Sell item.
	mPlayer->RemoveSkill(pItem->GetName());

	// Send message to server.
	SendItemRemoved(mPlayer->GetPlayer()->GetId(), pItem->GetName(), pItem->GetLevel());

	// Tell the shop that an item was sold.
	mShop->InventoryItemRemoved(pItem);

	// Free all slots and get the current skills from the player.
	UpdateItems();
}

void SkillInventory::UpdateItems()
{
	// Get the players skills.
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
	Player* player = GetClient()->GetLocalPlayer();
	player->SetGold(player->GetGold() + itemSlot.item->GetCost() - 3); // [NOTE][TODO] Hard coded!!!!

	// Send message to server.
	SendGoldChange(player->GetId(), player->GetGold());
}

string SkillInventory::GetHooverText(HudItem* pItem)
{
	char buffer[244];
	sprintf(buffer, "Sell value: %i gold\nLevel: %i\n", pItem->GetCost() - 3, pItem->GetLevel());	// [NOTE][TODO] Maybe enough?
	return string(buffer + pItem->GetDescription());
}

void SkillInventory::SetPlayer(PlayerModule* pPlayer)
{
	mPlayer = pPlayer;
	UpdateItems();
}

void SkillInventory::SetShop(Shop* pShop)
{
	mShop = pShop;
}