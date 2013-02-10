#include "Chat.h"
#include "StatusArea.h"
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
#include "NetworkMessages.h"
#include "StatusText.h"
#include "d3dUtil.h"
#include "D3DCore.h"
#include "UiCoordinate.h"
#include "HealthBar.h"
#include "RoundHandler.h"
#include "GameOverOverlay.h"

UserInterface::UserInterface(Client* pClient)
{
	mItemLoader = new ItemLoaderXML("items.xml");

	mStatusArea = new StatusArea(50, 50);
	mStatusArea->SetItemLoader(mItemLoader);
	mStatusArea->PlaceInFreeSlot(ItemKey(SKILL_FIREBALL, 1));
	mStatusArea->PlaceInFreeSlot(ItemKey(SKILL_FROSTNOVA, 1));
	mStatusArea->PlaceInFreeSlot(ItemKey(SKILL_METEOR, 1));

	mChat = new Chat(20, 740, 300, 200);
	mChat->SetClient(pClient);
	mChat->AddOnMessageSentListener(&UserInterface::OnMessageSent, this);

	mShop = new Shop(60, 770+75, 3, 60);
	mSkillShop = new Shop(360, 770+75, 3, 60);

	mInventory = new Inventory(1200, 770+75, 3, 60);
	mInventory->SetItemLoader(mItemLoader);
	mInventory->SetClient(pClient);
	mInventory->SetShop(mShop);

	mSkillInventory = new SkillInventory(1450, 755+75, 4, 42);
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
	mSkillShop->PlaceInFreeSlot(ItemKey(SKILL_FROSTNOVA, 1));
	mSkillShop->PlaceInFreeSlot(ItemKey(SKILL_TELEPORT, 1));
	mSkillShop->PlaceInFreeSlot(ItemKey(SKILL_METEOR, 1));
	mSkillShop->PlaceInFreeSlot(ItemKey(SKILL_HOOK, 1));

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/inventory_bkgd.png");

	mStatusText = new GLib::StatusText("nothing", 400, 200, 6);
	mHealthBar = new HealthBar(840, 800);

	mGameOverOverlay = nullptr;

	OnResize(GLib::GetClientWidth(), GLib::GetClientHeight());
}

UserInterface::~UserInterface()
{
	delete mChat;
	delete mShop;
	delete mInventory;
	delete mItemLoader;
	delete mStatusArea;
	delete mHealthBar;
}

void UserInterface::Update(GLib::Input* pInput, float dt)
{
	mInventory->Update(pInput, dt);
	mSkillInventory->Update(pInput, dt);
	mShop->Update(pInput, dt);
	mSkillShop->Update(pInput, dt);
	mStatusText->Update(dt);
	mStatusArea->Update(pInput, dt);

	if(mGameOverOverlay != nullptr)
		mGameOverOverlay->Update(pInput, dt);
}

void UserInterface::Draw(GLib::Graphics* pGraphics)
{
	// Background.
	UiCoordinate coords(UiAlignmentX::CENTER, BOTTOM, 800, 800, 1600, 200, false, true);
	pGraphics->DrawScreenQuad(mBkgdTexture, coords.x, coords.y, coords.width, coords.height);

	mInventory->Draw(pGraphics);
	mSkillInventory->Draw(pGraphics);
	mShop->Draw(pGraphics);
	mSkillShop->Draw(pGraphics);
	mStatusText->Draw(pGraphics);
	mStatusArea->Draw(pGraphics);
	mHealthBar->Draw(pGraphics);

	if(mGameOverOverlay != nullptr)
		mGameOverOverlay->Draw(pGraphics);
}

void UserInterface::HandleItemAdded(PlayerModule* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);
	
	HudItem* item = mItemLoader->GetItem(ItemKey(name, level));

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

	HudItem* item = mItemLoader->GetItem(ItemKey(name, level));

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

	// Request cvarlist with "-cvarlist".
	if(elems[0] == Cvars::CVAR_LIST_CMD + "\r\n") {
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)NMSG_REQUEST_CVAR_LIST);
		mInventory->GetClient()->SendServerMessage(bitstream);
	}
}

void UserInterface::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	mChat->MsgProc(msg, wParam, lParam);
}

void UserInterface::SetSelectedPlayer(PlayerModule* pPlayer)
{
	pPlayer->SetUserInterface(this);
	mInventory->SetPlayer(pPlayer->GetPlayer());
	mSkillInventory->SetPlayer(pPlayer);
	mHealthBar->SetSelectedPlayer(pPlayer->GetPlayer());
}

bool UserInterface::PointInsideUi(XMFLOAT3 position)
{
	return (position.y > GLib::GetClientHeight() - 200);
}

Chat* UserInterface::GetChat()
{
	return mChat;
}

void UserInterface::SetStatusText(string text, float time, float size, UINT32 color)
{
	mStatusText->SetText(text, time, color);
	mStatusText->SetSize(size);
}

void UserInterface::OnResize(float width, float height)
{
	UpdateChatPosition();

	mShop->OnResolutionChange();
	mSkillShop->OnResolutionChange();
	mInventory->OnResolutionChange();
	mSkillInventory->OnResolutionChange();
	mStatusArea->OnResolutionChange();
	mHealthBar->OnResolutionChange();

	if(mGameOverOverlay != nullptr)
		mGameOverOverlay->OnResize(width, height);
}

void UserInterface::UpdateChatPosition()
{
	mChat->SetDimensions(10, GLib::GetClientHeight()-460, 300, 200);
}

void UserInterface::OnStatusEffectAdded(ItemName type)
{
	mStatusArea->PlaceInFreeSlot(ItemKey(type, 1));
}

void UserInterface::OnStatusEffectRemoved(ItemName type)
{
	mStatusArea->RemoveStatusEffect(ItemKey(type, 1));
}

void UserInterface::DisplayGameOver(Client* pClient)
{
	auto scoreMap = pClient->GetRoundHandler()->GetScoreMap();

	mGameOverOverlay = new GameOverOverlay(500, 500);
	mGameOverOverlay->SetScoreMap(scoreMap);
}