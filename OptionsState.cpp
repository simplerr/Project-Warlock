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
#include "Button.h"
#include <windows.h>
#include "UiCoordinate.h"
#include <fstream>
#include <stdio.h>

OptionsState OptionsState::mOptionsState;

// The default edit control procedure.
WNDPROC DefNameEditProc;
#define ID_UPDATE_NAME 101
#define IDC_INPUT_BOX  102

//! Subclassed msg proc for the input box.
LRESULT NameInputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	bool enter = false;
	switch (uMsg)
	{
	case WM_CHAR:
		// Enter was pressed.
		if(wParam == VK_RETURN)
			enter = true;
	default:
		// Handles all default actions.
		result =  CallWindowProc(DefNameEditProc, hwnd, uMsg, wParam, lParam);
	}

	// Has to be here since case WM_CHAR is before the default procedure. 
	// Otherwise the caret positions gets set but then changed when the ENTER msg is proccessed.
	if(enter)
	{
		// Send the message so Chat can catch it.
		SendMessage(GLib::GetWindowHandler(), ID_UPDATE_NAME, 0, 0);
	}

	return result;
}

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
	mControlManager->Draw(pGraphics);
}

void OptionsState::BuildUi()
{
	Label* title = new Label(800, 50, "StateHeader", "Options");
	mControlManager->AddControl(title);

	/*TextMenu* serverMenu = new TextMenu(800, 400, "MainMenu");
	serverMenu->AddItemPressedListener(&OptionsState::OnMenuItemPressed, this);
	serverMenu->PerformLayout();
	mControlManager->AddControl(serverMenu);*/

	// Add the player name edit box.
	mhNameBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
		700, 250, 200, 30, GLib::GetWindowHandler(), (HMENU)IDC_INPUT_BOX, GLib::GetAppInstance(), NULL);

	// Set the default edit control proc
	DefNameEditProc = (WNDPROC)SetWindowLong(mhNameBox, GWL_WNDPROC, (DWORD)NameInputProc);

	// Add a back button.
	mBackButton = new Button(905, 510, "OptionsBackButton", "Back");
	mBackButton->AddPressedListener(&OptionsState::OnButtonPressed, this);

	mControlManager->AddControl(mBackButton);
	mBackButton->SetAlignment(false, true);
	
	mControlManager->LoadLuaProperties();

	FILE* file = fopen("config.txt", "rt");
	char name[256];
	fgets(name, 255, file);
	fclose(file);

	// Load the nickname from the config file.
	SetWindowText(mhNameBox, name);
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
		// Load the ip so we can write it back.
		/*std::ifstream fin("config.txt");
		string ip;
		fin >> ip >> ip;
		fin.close();

		std::ofstream fout("config.txt");
		char buffer[256];
		fout << GetWindowText(mhNameBox, buffer, 255) << endl;;
		fout << ip;
		fout.close();*/

		FILE* fin = fopen("config.txt", "rt");
		char ip[256];
		fgets(ip, 255, fin);
		fgets(ip, 255, fin);
		fclose(fin);

		FILE* file = fopen("config.txt", "w");
		char name[256];
		GetWindowText(mhNameBox, name, 255);

		if(string(name).find_first_of('\n') == string::npos)
			fprintf(file, "%s\n%s", name, ip);
		else
			fprintf(file, "%s%s", name, ip);

		fclose(file);

		ChangeState(MainMenuState::Instance());
	}
}