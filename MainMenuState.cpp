#include "MainMenuState.h"
#include "Graphics.h"
#include "Input.h"
#include "PlayingState.h"
#include "ControlManager.h"
#include "TextMenu.h"
#include "Label.h"
#include "Client.h"
#include "LobbyState.h"
#include "Database.h"
#include "Game.h"
#include "BrowsingState.h"
#include "OptionsState.h"
#include "HostState.h"
#include "Sound.h"

MainMenuState MainMenuState::mMainMenuState;

void MainMenuState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("data/lua/ui_layout.lua");

	BuildUi();

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("data/textures/menu_bkgd.png");
	mTitleTexture = GLib::GetGraphics()->LoadTexture("data/textures/menu_title.png");

	gSound->PlayMusic("data/data/sounds/menu_music.wav", true, false);
	mChageToState = "none";
}

void MainMenuState::Cleanup(void)
{
	delete mControlManager;
}

void MainMenuState::Pause()
{

}

void MainMenuState::Resume()
{

}

void MainMenuState::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

void MainMenuState::Update(GLib::Input* pInput, double dt)
{
	mControlManager->Update(pInput, dt);

	if(mChageToState == "Play")
	{
		ChangeState(BrowsingState::Instance());
		/*mMenu->ClearItems();

		mMenu->AddItem("Host", "Host");
		mMenu->AddItem("Browse", "Browse");
		mMenu->PerformLayout();*/
	}
	else if(mChageToState == "Options")
		ChangeState(OptionsState::Instance());
	else if(mChageToState == "About")
	{

	}
	else if(mChageToState == "Browse")
		ChangeState(BrowsingState::Instance());
	else if(mChageToState == "Host")
		ChangeState(HostState::Instance());
}

void MainMenuState::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, GLib::GetClientWidth()/2, GLib::GetClientHeight()/2, GLib::GetClientWidth(), GLib::GetClientHeight());
	pGraphics->DrawScreenQuad(mTitleTexture, GLib::GetClientWidth()/2, 130, 798, 239);
	mControlManager->Draw(pGraphics);
}

void MainMenuState::BuildUi()
{
	mMenu = new TextMenu(800, 400, "MainMenu");
	mMenu->AddItemPressedListener(&MainMenuState::OnMenuItemPressed, this);

	mMenu->AddItem("Play", "Play");
	mMenu->AddItem("Options", "Options");
	mMenu->AddItem("About", "About");
	
	mMenu->SetPressedSound("data/sounds/button.wav");

	mMenu->PerformLayout();
	mControlManager->AddControl(mMenu);
	mControlManager->LoadLuaProperties();
}

void MainMenuState::OnResize(float width, float height)
{
	mControlManager->OnResize(width, height);
}

void MainMenuState::OnMenuItemPressed(Label* pLabel)
{
	mChageToState = pLabel->GetName();
}