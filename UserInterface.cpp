#include "Chat.h"
#include "UserInterface.h"
#include "ItemLoaderXML.h"
#include "Inventory.h"
#include "Shop.h"
#include "Client.h"
#include "Player.h"
#include "Graphics.h"
#include "SkillInventory.h"
#include "ServerCvars.h"
#include "PlayerModule.h"

UserInterface::UserInterface(Client* pClient)
{
	mItemLoader = new ItemLoaderXML("items.xml");

	mChat = new Chat(20, 440, 300, 200);
	mChat->SetClient(pClient);
	mChat->AddOnMessageSentListener(&UserInterface::OnMessageSent, this);

	mShop = new Shop(60, 770, 3, 60);
	mSkillShop = new Shop(360, 770, 3, 60);

	mInventory = new Inventory(700, 770, 3, 60);
	mInventory->SetItemLoader(mItemLoader);
	mInventory->SetClient(pClient);
	mInventory->SetShop(mShop);

	mSkillInventory = new SkillInventory(950, 755, 4, 42);
	mSkillInventory->SetItemLoader(mItemLoader);
	mSkillInventory->SetClient(pClient);
	mSkillInventory->SetShop(mSkillShop);
	
	mShop->SetClient(pClient);
	mShop->SetItemLoader(mItemLoader);
	mShop->SetInspectingInventory(mInventory);
	mShop->PlaceInFreeSlot(ItemKey(REGEN_CAP, 1));
	mShop->PlaceInFreeSlot(ItemKey(IRON_ARMOR, 1));

	mSkillShop->SetClient(pClient);
	mSkillShop->SetItemLoader(mItemLoader);
	mSkillShop->SetInspectingInventory(mSkillInventory);
	mSkillShop->PlaceInFreeSlot(ItemKey(SKILL_FIREBALL, 1));

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/ui_bkgd.png");
}

UserInterface::~UserInterface()
{
	delete mChat;
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

void UserInterface::HandleItemAdded(PlayerModule* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);
	
	BaseItem* item = mItemLoader->GetItem(ItemKey(name, level));

	if(!item->IsSkill()) {
		pPlayer->GetPlayer()->AddItem(mItemLoader, ItemKey(name, level));
		mInventory->UpdateItems();
	}
	else {
		Skill* skill = pPlayer->AddSkill(name);
		skill->SetAttributes(item->GetAttributes());	// [HACK]!!
		skill->SetDescription(item->GetDescription());
		skill->SetCost(item->GetCost());
		skill->SetLevel(item->GetLevel());

		mSkillInventory->UpdateItems();
	}
}

void UserInterface::HandleItemRemoved(PlayerModule* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);

	BaseItem* item = mItemLoader->GetItem(ItemKey(name, level));

	if(!item->IsSkill()) {
		pPlayer->GetPlayer()->RemoveItem(item);
		mInventory->UpdateItems();
	}
	else {
		pPlayer->RemoveSkill(item->GetName());
		mSkillInventory->UpdateItems();
	}
}

void UserInterface::HandleChatMessage(RakNet::BitStream& bitstream)
{
	mChat->HandleMessage(bitstream);
}

void UserInterface::HandleAddChatText(RakNet::BitStream& bitstream)
{
	char buffer[1024];
	COLORREF color;

	bitstream.Read(buffer);
	bitstream.Read(color);

	mChat->AddText(buffer, color);
}

// Callback from Chat.
void UserInterface::OnMessageSent(string message)
{
	vector<string> elems = GLib::SplitString(message, ' ');

	if(elems[0] == Cvars::CVAR_LIST_CMD + "\r\n") {
		mInventory->GetClient()->RequestCvarList(); // [NOTE][HACK] !!
	}
}

void UserInterface::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	mChat->MsgProc(msg, wParam, lParam);
}

void UserInterface::SetSelectedPlayer(PlayerModule* pPlayer)
{
	mInventory->SetPlayer(pPlayer->GetPlayer());
	mSkillInventory->SetPlayer(pPlayer);
}

bool UserInterface::PointInsideUi(XMFLOAT3 position)
{
	return (position.y > GLib::GetClientHeight() - 200);
}

Chat* UserInterface::GetChat()
{
	return mChat;
}