#include "PlayingState.h"
#include "Graphics.h"
#include "Client.h"
#include "TextMenu.h"
#include "ControlManager.h"
#include "Input.h"
#include "BrowsingState.h"

PlayingState PlayingState::mPlayingState;

void PlayingState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	// Create the peer.
	mClient = new Client();

	mControlManager = new ControlManager("ui_layout.lua");

	/*TextMenu* menu = new TextMenu(300, 300, "TextMenu");
	menu->AddItem("test_label", "teXTING");
	menu->AddItem("test_label2", "hehe");
	menu->AddItem("test_label3", "h");
	menu->AddItem("test_label4", "kanske en apa");
	mControlManager->AddControl(menu);
	mControlManager->LoadLuaProperties();*/
}

void PlayingState::Cleanup(void)
{
	delete mClient;
	delete mControlManager;

	mClient = nullptr;
}

void PlayingState::Update(GLib::Input* pInput, double dt)
{
	mClient->Update(pInput, dt);
	mControlManager->Update(pInput, dt);

	if(pInput->KeyPressed(VK_SPACE))
		ChangeState(BrowsingState::Instance());
}

void PlayingState::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawText("PlayingState", 600, 400, 60);

	mClient->Draw(pGraphics);
	mControlManager->Draw(pGraphics);
}

void PlayingState::Pause()
{

}

void PlayingState::Resume()
{

}

void PlayingState::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(mClient != nullptr)
		mClient->MsgProc(msg, wParam, lParam);
}

Client* PlayingState::GetClient()
{
	return mClient;
}