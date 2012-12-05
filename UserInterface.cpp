#include "UserInterface.h"
#include "ItemLoaderXML.h"
#include "Inventory.h"
#include "Shop.h"
#include "Client.h"
#include "Player.h"
#include "Graphics.h"
#include "SkillInventory.h"

UserInterface::UserInterface(Client* pClient)
{
	mItemLoader = new ItemLoaderXML("items.xml");

	mInventory = new Inventory(700, 770, 3, 60);
	mInventory->SetItemLoader(mItemLoader);
	mInventory->SetClient(pClient);

	mSkillInventory = new SkillInventory(950, 755, 4, 42);
	mSkillInventory->SetItemLoader(mItemLoader);
	mSkillInventory->SetClient(pClient);

	mShop = new Shop(60, 770, 3, 60);
	mShop->SetClient(pClient);
	mShop->SetItemLoader(mItemLoader);
	mShop->SetInspectingInventory(mInventory);
	mShop->PlaceInFreeSlot(ItemKey(REGEN_CAP, 1));
	mShop->PlaceInFreeSlot(ItemKey(IRON_ARMOR, 1));

	mSkillShop = new Shop(360, 770, 3, 60);
	mSkillShop->SetClient(pClient);
	mSkillShop->SetItemLoader(mItemLoader);
	mSkillShop->SetInspectingInventory(mSkillInventory);
	mSkillShop->PlaceInFreeSlot(ItemKey(SKILL_FIREBALL, 1));

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/ui_bkgd.png");
}

UserInterface::~UserInterface()
{
	delete mShop;
	delete mInventory;
	delete mItemLoader;
}

void UserInterface::Update(GLib::Input* pInput, float dt)
{
	mInventory->Update(pInput, dt);
	mSkillInventory->Update(pInput, dt);
	mShop->Update(pInput, dt);
	mSkillShop->Update(pInput, dt);
}

void UserInterface::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, 600, 800, 1200, 200);
	mInventory->Draw(pGraphics);
	mSkillInventory->Draw(pGraphics);
	mShop->Draw(pGraphics);
	mSkillShop->Draw(pGraphics);
}

void UserInterface::HandleItemAdded(Player* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);
	
	BaseItem* item = mItemLoader->GetItem(ItemKey(name, level));

	if(!item->IsSkill()) {
		pPlayer->AddItem(mItemLoader, ItemKey(name, level));
		mInventory->UpdateItems();
		//mInventory->AddItem(item);
	}
	else {
		pPlayer->AddSkill(name);
		mSkillInventory->UpdateItems();
		//mSkillInventory->AddItem(item);
	}
}

void UserInterface::HandleItemRemoved(Player* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);

	BaseItem* item = mItemLoader->GetItem(ItemKey(name, level));

	if(!item->IsSkill()) {
		pPlayer->RemoveItem(mItemLoader, ItemKey(name, level));
		mInventory->UpdateItems();
	}
	else {
		pPlayer->RemoveSkill(item->GetName());
		mSkillInventory->UpdateItems();
	}
}

void UserInterface::SetSelectedPlayer(Player* pPlayer)
{
	mInventory->SetPlayer(pPlayer);
	mSkillInventory->SetPlayer(pPlayer);
}

bool UserInterface::PointInsideUi(XMFLOAT3 position)
{
	return (position.y > GLib::GetClientHeight() - 200);
}