#include "OptionsState.h"
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
#include "MainMenuState.h"
#include <Windowsx.h>
#include <Richedit.h>
#include <Commctrl.h>
#include <windows.h>
#include "Button.h"
#include "UiCoordinate.h"
#include <fstream>
#include <stdio.h>
#include "Config.h"

OptionsState OptionsState::mOptionsState;

#define ID_UPDATE_NAME 101
#define IDC_NICKNAME_BOX  102
#define IDC_SERVERNAME_BOX  103

void OptionsState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("ui_layout.lua");
	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/menu_bkgd.png");
	mWhiteTexture = GLib::GetGraphics()->LoadTexture("textures/white_transparent.png");

	BuildUi();
}

void OptionsState::Cleanup(void)
{
	DestroyWindow(mhNameBox);
	DestroyWindow(mhServerNameBox);
}

void OptionsState::Pause()
{

}

void OptionsState::Resume()
{

}

void OptionsState::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

void OptionsState::Update(GLib::Input* pInput, double dt)
{
	mControlManager->Update(pInput, dt);
}

void OptionsState::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, GLib::GetClientWidth()/2, GLib::GetClientHeight()/2, GLib::GetClientWidth(), GLib::GetClientHeight());
	pGraphics->DrawScreenQuad(mWhiteTexture, GLib::GetClientWidth()/2, 330, 300, 300);

	pGraphics->DrawText("Nickname", GLib::GetClientWidth()/2-100, 220, 20, GLib::ColorRGBA(0, 0, 0, 255));
	pGraphics->DrawText("Server name", GLib::GetClientWidth()/2-100, 320, 20, GLib::ColorRGBA(0, 0, 0, 255));
	mControlManager->Draw(pGraphics);
}

void OptionsState::BuildUi()
{
	Label* title = new Label(800, 50, "StateHeader", "Options");
	mControlManager->AddControl(title);

	mhNameBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
		GLib::GetClientWidth()/2-100, 250, 200, 30, GLib::GetWindowHandler(), (HMENU)IDC_NICKNAME_BOX, GLib::GetAppInstance(), NULL);

	mhServerNameBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
		GLib::GetClientWidth()/2-100, 350, 200, 30, GLib::GetWindowHandler(), (HMENU)IDC_SERVERNAME_BOX, GLib::GetAppInstance(), NULL);

	// Add a back button.
	mBackButton = new Button(905, 510, "OptionsBackButton", "Back");
	mBackButton->AddPressedListener(&OptionsState::OnButtonPressed, this);

	mControlManager->AddControl(mBackButton);
	mBackButton->SetAlignment(false, true);
	
	mControlManager->LoadLuaProperties();

	// Load the nickname from the config file.
	Config config("config.txt");
	SetWindowText(mhNameBox, config.nickName.c_str());
	SetWindowText(mhServerNameBox, config.serverName.c_str());
}

void OptionsState::OnResize(float width, float height)
{
	/*UiCoordinate coord(UiAlignmentX::RIGHT, UiAlignmentY::TOP, mBackButton->GetPosition().x, mBackButton->GetPosition().y, mBackButton->GetRect().Width(), mBackButton->GetRect().Height(), false, false);
	mBackButton->SetPosition(coord.x, coord.y);*/

	mControlManager->OnResize(width, height);
	SetWindowPos(mhNameBox, HWND_TOP, GLib::GetClientWidth()/2-100, 250, 200, 30, SWP_SHOWWINDOW);
}

void OptionsState::OnButtonPressed(Button* pButton)
{
	if(pButton->GetName() == "OptionsBackButton")
	{
		Config config("config.txt");
		char buffer[256];
		GetWindowText(mhNameBox, buffer, 255);
		config.nickName = buffer;

		GetWindowText(mhServerNameBox, buffer, 255);
		config.serverName = buffer;
		
		config.Save();

		ChangeState(MainMenuState::Instance());
	}
}