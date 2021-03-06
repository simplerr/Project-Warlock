#include "PlayingState.h"
#include "Graphics.h"
#include "Client.h"
#include "TextMenu.h"
#include "ControlManager.h"
#include "Input.h"
#include "BrowsingState.h"
#include "Chat.h"
#include "Camera.h"
#include "UserInterface.h"
#include "Sound.h"
#include "Config.h"
#include "Graphics.h"
#include "Effects.h"
#include "ServerCvars.h"

PlayingState PlayingState::mPlayingState;

void PlayingState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);
	//gSound->PlayMusic("data/sounds/ambient.wav", true, 0);
	gSound->SetMusicVolume(0.1f);

	Config config("data/config.txt");
	GLib::GetGraphics()->GetCamera()->SetMovementSpeed(config.lookSense);

	GLib::Effects::TerrainFX->SetArenaRadius(gCvars->GetCvarValue(Cvars::ARENA_RADIUS));
}

void PlayingState::Cleanup(void)
{
	delete mClient;
	mClient = nullptr;
}

void PlayingState::Update(GLib::Input* pInput, double dt)
{
	mClient->Update(pInput, dt);

	//GLib::GetGraphics()->GetCamera()->Update(pInput, dt);
}

void PlayingState::Draw(GLib::Graphics* pGraphics)
{
	mClient->Draw(pGraphics);
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