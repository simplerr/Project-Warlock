#include "GameOverOverlay.h"
#include "Graphics.h"
#include "TextMenu.h"
#include "ControlManager.h"
#include "Button.h"
#include "UserInterface.h"
#include "Sound.h"
#include "Client.h"
#include "NetworkMessages.h"

GameOverOverlay::GameOverOverlay(float x, float y, Client* pClient)
{
	mScoreList = new TextMenu(x, y, "GameOverList");
	mScoreList->SetAlignment(false, true);
	mControlManager = new ControlManager("data/lua/ui_layout.lua");
	mControlManager->AddControl(mScoreList);
	mControlManager->LoadLuaProperties();

	mChangeState = false;

	mClient = pClient;

	gSound->PlayEffect("data/sounds/game_over_screen.wav");
}

GameOverOverlay::~GameOverOverlay()
{
	delete mControlManager;
}

void GameOverOverlay::Update(GLib::Input* pInput, float dt)
{
	mControlManager->Update(pInput, dt);
}

void GameOverOverlay::Draw(GLib::Graphics* pGraphics)
{
	string text = mWinner + " wins the game!";
	float width = pGraphics->MeasureText(text, 60, "Arial").Width();
	pGraphics->DrawText(text, GLib::GetClientWidth()/2-width/2, 80, 60);

	mControlManager->Draw(pGraphics);
}

void GameOverOverlay::SetScoreMap(map<string, int> scoreMap)
{
	for(auto iter = scoreMap.begin(); iter != scoreMap.end(); iter++)
	{
		string nameAndScore;
		nameAndScore.resize(50, ' ');
		nameAndScore.replace(0, (*iter).first.length(), (*iter).first);
		char score[10];
		sprintf(score, "%i", (*iter).second);
		nameAndScore.replace(30, string(score).length(), score);

		mScoreList->AddItem(nameAndScore, nameAndScore);
	}

	// Add "Back to main menu" button.
	mMainMenuButton = new Button(100, 100, "GameOverButton" ,"Main Menu");
	mMainMenuButton->SetBkgdScale(1.2f);
	mMainMenuButton->AddPressedListener(&GameOverOverlay::ButtonPressed, this);
	mMainMenuButton->SetPressedSound("data/sounds/button.wav");
	mControlManager->AddControl(mMainMenuButton);

	// Add "Back to main menu" button.
	mRematchButton = new Button(100, 100, "GameOverButton" ,"Rematch");
	mRematchButton->SetBkgdScale(1.2f);
	mRematchButton->AddPressedListener(&GameOverOverlay::ButtonPressed, this);
	mRematchButton->SetPressedSound("data/sounds/button.wav");
	mControlManager->AddControl(mRematchButton);

	if(!mClient->IsHost())
		mRematchButton->SetDisabled(true);

	mControlManager->LoadLuaProperties();

	GLib::Rect scoreRect = mScoreList->GetRect();
	float right = (scoreRect.left + scoreRect.Width()/2) + scoreRect.Width()*mScoreList->GetBkgdScale()/2;
	mMainMenuButton->SetPosition(right - mMainMenuButton->GetRect().Width()/2, scoreRect.bottom + mMainMenuButton->GetRect().Height()/2 + 20);

	mRematchButton->SetPosition(mMainMenuButton->GetPosition().x - 200, mMainMenuButton->GetPosition().y);
}

void GameOverOverlay::OnResize(float width, float height)
{
	mControlManager->OnResize(width, height);
	mControlManager->LoadLuaProperties();

	GLib::Rect scoreRect = mScoreList->GetRect();
	float right = (scoreRect.left + scoreRect.Width()/2) + scoreRect.Width()*mScoreList->GetBkgdScale()/2;
	mMainMenuButton->SetPosition(right - mMainMenuButton->GetRect().Width()/2, scoreRect.bottom + mMainMenuButton->GetRect().Height()/2);
}

void GameOverOverlay::ButtonPressed(Button* pButton)
{
	if(pButton->GetText() == "Main Menu")
	{
		mChangeState = true;
		// Turn off the server is host. [NOTE][TODO]
	}
	else if(pButton->GetText() == "Rematch")
	{
		// Send rematch request.
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)NMSG_REQUEST_REMATCH);
		mClient->SendServerMessage(bitstream);
	}
}

void GameOverOverlay::SetUserInterface(UserInterface* pInterface)
{
	mUserInterface = pInterface;
}

bool GameOverOverlay::GetChangeState()
{
	return mChangeState;
}

void GameOverOverlay::SetWinner(string winner)
{
	mWinner = winner;
}