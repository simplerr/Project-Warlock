#include "Chat.h"
#include "AttributesUi.h"
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
#include "InGameMenu.h"
#include "MainMenuState.h"
#include "PlayingState.h"

UserInterface::UserInterface(Client* pClient)
{
	SetReady(false);

	mItemLoader = new ItemLoaderXML("items.xml");

	mStatusArea = new StatusArea(50, 50);
	mStatusArea->SetItemLoader(mItemLoader);

	mShop = new Shop(50, 200, 3, 60);
	mSkillShop = new Shop(50, 380, 3, 60);

	mInventory = new Inventory(1100, 770+75, 3, 60);
	mInventory->SetItemLoader(mItemLoader);
	mInventory->SetClient(pClient);
	mInventory->SetShop(mShop);

	mSkillInventory = new SkillInventory(1350, 770+75, 4, 60);
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

	mStatusText = new GLib::StatusText("nothing", 800, 200, 6);
	mHealthBar = new HealthBar(840, 800);

	mGameOverOverlay = nullptr;

	mInGameMenu = new InGameMenu("ui_layout.lua");

	mChat = new Chat(20, 740, 300, 200);
	mChat->SetClient(pClient);
	mChat->AddOnMessageSentListener(&UserInterface::OnMessageSent, this);

	mAttributesUi = new AttributesUi(170, 800);

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
	delete mInGameMenu;
	delete mAttributesUi;
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

	mInGameMenu->Update(pInput, dt);

	// Change the state from the in-game menu and the game over display.
	if((mGameOverOverlay != nullptr && mGameOverOverlay->GetChangeState()) || mInGameMenu->GetChangeState())
		PlayingState::Instance()->ChangeState(MainMenuState::Instance());
}

void UserInterface::Draw(GLib::Graphics* pGraphics)
{
	// Background.
	UiCoordinate coords(UiAlignmentX::CENTER, BOTTOM, 800, 800, 1600, 200, false, true);
	pGraphics->DrawScreenQuad(mBkgdTexture, coords.x, coords.y, coords.width, coords.height);

	mAttributesUi->Draw(pGraphics);
	mInventory->Draw(pGraphics);
	mSkillInventory->Draw(pGraphics);
	mShop->Draw(pGraphics);
	mSkillShop->Draw(pGraphics);
	mStatusText->Draw(pGraphics);
	mStatusArea->Draw(pGraphics);
	mHealthBar->Draw(pGraphics);
	mInGameMenu->Draw(pGraphics);

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
	if(mChat != nullptr)
		mChat->MsgProc(msg, wParam, lParam);
}

void UserInterface::SetSelectedPlayer(PlayerModule* pPlayer)
{
	pPlayer->SetUserInterface(this);
	mInventory->SetPlayer(pPlayer->GetPlayer());
	mSkillInventory->SetPlayer(pPlayer);
	mHealthBar->SetSelectedPlayer(pPlayer->GetPlayer());
	mAttributesUi->SetSelectedPlayer(pPlayer->GetPlayer());
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
	mInGameMenu->OnResize(width, height);
	mStatusText->SetPosition(GLib::GetClientWidth()/2, mStatusText->GetPosition().y);

	if(mGameOverOverlay != nullptr)
		mGameOverOverlay->OnResize(width, height);

	mSkillInventory->SetPosition(width - mSkillInventory->GetWidth(), mSkillInventory->GetPosition().y);
	mInventory->SetPosition(width - mInventory->GetWidth() - mSkillInventory->GetWidth() - 30, mInventory->GetPosition().y);
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

void UserInterface::DisplayGameOver(Client* pClient, string winner)
{
	auto scoreMap = pClient->GetRoundHandler()->GetScoreMap();

	mGameOverOverlay = new GameOverOverlay(500, 500);
	mGameOverOverlay->SetScoreMap(scoreMap);
	mGameOverOverlay->SetWinner(winner);
}

void UserInterface::SetReady(bool ready)
{
	mIsReady = ready;
}

bool UserInterface::IsReady()
{
	return mIsReady;
}