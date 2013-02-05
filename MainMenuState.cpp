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

MainMenuState MainMenuState::mMainMenuState;

void MainMenuState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("ui_layout.lua");

	BuildUi();

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/menu_bkgd.png");
}

void MainMenuState::Cleanup(void)
{

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
}

void MainMenuState::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, GLib::GetClientWidth()/2, GLib::GetClientHeight()/2, GLib::GetClientWidth(), GLib::GetClientHeight());
	mControlManager->Draw(pGraphics);
}

void MainMenuState::BuildUi()
{
	Label* title = new Label(800, 50, "StateHeader", "Project Warlock");
	mControlManager->AddControl(title);

	TextMenu* serverMenu = new TextMenu(800, 400, "MainMenu");
	serverMenu->AddItemPressedListener(&MainMenuState::OnMenuItemPressed, this);

	serverMenu->AddItem("Play", "Play");
	serverMenu->AddItem("Options", "Options");
	serverMenu->AddItem("About", "About");
	
	serverMenu->PerformLayout();
	mControlManager->AddControl(serverMenu);
	mControlManager->LoadLuaProperties();
}

void MainMenuState::OnResize(float width, float height)
{
	mControlManager->OnResize(width, height);
}

void MainMenuState::OnMenuItemPressed(Label* pLabel)
{
	if(pLabel->GetName() == "Play")
	{
		ChangeState(BrowsingState::Instance());
	}
	else if(pLabel->GetName() == "Options")
	{
		ChangeState(OptionsState::Instance());
	}
	else if(pLabel->GetName() == "About")
	{

	}
}