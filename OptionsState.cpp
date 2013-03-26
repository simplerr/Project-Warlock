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
#define IDC_LOOKSENSE_BOX 104
#define IDC_MUSIC_BOX 105
#define IDC_EFFECTS_BOX 106

void OptionsState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("data/lua/ui_layout.lua");
	mBkgdTexture = GLib::GetGraphics()->LoadTexture("data/textures/menu_bkgd.png");
	mWhiteTexture = GLib::GetGraphics()->LoadTexture("data/textures/white_transparent.png");
	mChangeState = "none";

	BuildUi();
}

void OptionsState::Cleanup(void)
{
	DestroyWindow(mhNameBox);
	DestroyWindow(mhServerNameBox);
	DestroyWindow(mhLookSensBox);
	
	delete mControlManager;
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

	if(mChangeState == "OptionsBackButton")
	{
		Config config("data/config.txt");
		char buffer[256];
		GetWindowText(mhNameBox, buffer, 255);
		config.nickName = buffer;

		GetWindowText(mhServerNameBox, buffer, 255);
		config.serverName = buffer;

		GetWindowText(mhLookSensBox, buffer, 255);

		if(GLib::IsNumber(string(buffer))) {
			config.lookSense = atof(buffer);
		}
		else 
			config.lookSense = 1.0f;

		
		config.music = (Button_GetCheck(mhMusicCheckbox) == BST_CHECKED ? 1 : 0); 
		config.soundEffects = (Button_GetCheck(mhEffectsChecbox) == BST_CHECKED ? 1 : 0); 

		config.Save();

		ChangeState(MainMenuState::Instance());
	}
}

void OptionsState::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, GLib::GetClientWidth()/2, GLib::GetClientHeight()/2, GLib::GetClientWidth(), GLib::GetClientHeight());
	pGraphics->DrawScreenQuad(mWhiteTexture, GLib::GetClientWidth()/2, 400, 300, 400);

	pGraphics->DrawText("Nickname", GLib::GetClientWidth()/2-100, 220, 20, GLib::ColorRGBA(0, 0, 0, 255));
	pGraphics->DrawText("Server name", GLib::GetClientWidth()/2-100, 320, 20, GLib::ColorRGBA(0, 0, 0, 255));
	pGraphics->DrawText("Look sense (0 - 10)", GLib::GetClientWidth()/2-100, 420, 20, GLib::ColorRGBA(0, 0, 0, 255));
	//pGraphics->DrawText("Music", GLib::GetClientWidth()/2-100, 520, 20, GLib::ColorRGBA(0, 0, 0, 255));
	//pGraphics->DrawText("Sound effects", GLib::GetClientWidth()/2-100, 620, 20, GLib::ColorRGBA(0, 0, 0, 255));
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

	mhLookSensBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
		GLib::GetClientWidth()/2-100, 450, 200, 30, GLib::GetWindowHandler(), (HMENU)IDC_LOOKSENSE_BOX, GLib::GetAppInstance(), NULL);

	/*mhMusicCheckbox = CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX  | WS_OVERLAPPED,
		GLib::GetClientWidth()/2-100, 550, 15, 15, GLib::GetWindowHandler(), (HMENU)IDC_MUSIC_BOX, GLib::GetAppInstance(), NULL);

	mhEffectsChecbox = CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_OVERLAPPED,
		GLib::GetClientWidth()/2-100, 650, 15, 15, GLib::GetWindowHandler(), (HMENU)IDC_EFFECTS_BOX, GLib::GetAppInstance(), NULL);*/

	// Add a back button.
	mBackButton = new Button(905, 510, "OptionsBackButton", "Back");
	mBackButton->AddPressedListener(&OptionsState::OnButtonPressed, this);

	mControlManager->AddControl(mBackButton);
	mBackButton->SetAlignment(false, true);
	mBackButton->SetPressedSound("data/sounds/button.wav");
	
	mControlManager->LoadLuaProperties();

	// Load the nickname from the config file.
	Config config("data/config.txt");
	SetWindowText(mhNameBox, config.nickName.c_str());
	SetWindowText(mhServerNameBox, config.serverName.c_str());
	char buffer[32];
	sprintf(buffer, "%.1f", config.lookSense);
	SetWindowText(mhLookSensBox, buffer);

	SendMessage(mhMusicCheckbox, BM_SETCHECK, config.music, 0);
	SendMessage(mhEffectsChecbox, BM_SETCHECK, config.soundEffects, 0);
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
		mChangeState = "OptionsBackButton";

}