#include "PlayingState.h"
#include "Graphics.h"
#include "Client.h"
#include "TextMenu.h"
#include "ControlManager.h"
#include "Input.h"
#include "BrowsingState.h"
#include "Chat.h"
#include "UserInterface.h"

PlayingState PlayingState::mPlayingState;

void PlayingState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("ui_layout.lua");
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

	//if(pInput->KeyPressed(VK_SPACE))
	//	ChangeState(BrowsingState::Instance());
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

void PlayingState::SetClient(Client* pClient)
{
	mClient = pClient;
	mClient->GetUi()->UpdateChatPosition();
}

void PlayingState::SetDatabase(Database* pDatabase)
{

}

void PlayingState::OnResize(float width, float height)
{
	mClient->GetUi()->OnResize(width, height);
}