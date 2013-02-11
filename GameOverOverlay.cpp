#include "GameOverOverlay.h"
#include "Graphics.h"
#include "TextMenu.h"
#include "ControlManager.h"
#include "Button.h"
#include "UserInterface.h"

GameOverOverlay::GameOverOverlay(float x, float y)
{
	mScoreList = new TextMenu(x, y, "GameOverList");
	mScoreList->SetAlignment(false, true);
	mControlManager = new ControlManager("ui_layout.lua");
	mControlManager->AddControl(mScoreList);
	mControlManager->LoadLuaProperties();

	mChangeState = false;
}

GameOverOverlay::~GameOverOverlay()
{

}

void GameOverOverlay::Update(GLib::Input* pInput, float dt)
{
	mControlManager->Update(pInput, dt);
}

void GameOverOverlay::Draw(GLib::Graphics* pGraphics)
{
	string text = "Game over!";
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
	mControlManager->AddControl(mMainMenuButton);
	mControlManager->LoadLuaProperties();

	GLib::Rect scoreRect = mScoreList->GetRect();
	float right = (scoreRect.left + scoreRect.Width()/2) + scoreRect.Width()*mScoreList->GetBkgdScale()/2;
	mMainMenuButton->SetPosition(right - mMainMenuButton->GetRect().Width()/2, scoreRect.bottom + mMainMenuButton->GetRect().Height()/2);
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
	mChangeState = true;
	// Turn off the server is host. [NOTE][TODO]
}

void GameOverOverlay::SetUserInterface(UserInterface* pInterface)
{
	mUserInterface = pInterface;
}

bool GameOverOverlay::GetChangeState()
{
	return mChangeState;
}