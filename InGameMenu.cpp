#include "InGameMenu.h"
#include "Input.h"
#include "TextMenu.h"
#include "Label.h"
#include "PlayingState.h"
#include "MainMenuState.h"

InGameMenu::InGameMenu(string luaScript)
	: ControlManager(luaScript)
{
	mMenu = new TextMenu(0, 0, "InGameMenu");
	mMenu->AddItem("MainMenu", "Exit to Main Menu");
	mMenu->SetVisible(false);
	mMenu->SetPressedSound("sounds/button.wav");
	mMenu->AddItemPressedListener(&InGameMenu::ButtonPressed, this);
	AddControl(mMenu);

	mChangeState = false;

	LoadLuaProperties();
	mMenu->PerformLayout();
	mMenu->SetPosition(800, 200);
}

InGameMenu::~InGameMenu()
{

}

void InGameMenu::Update(GLib::Input* pInput, float dt)
{
	ControlManager::Update(pInput, dt);

	if(pInput->KeyPressed(VK_ESCAPE))
		mMenu->SetVisible(!mMenu->GetVisible());
}

void InGameMenu::Draw(GLib::Graphics* pGraphics)
{
	ControlManager::Draw(pGraphics);
}

void InGameMenu::ButtonPressed(Label* pButton)
{
	if(pButton->GetName() == "MainMenu") 
		mChangeState = true;
}

bool InGameMenu::GetChangeState()
{
	return mChangeState;
}

void InGameMenu::SetVisible(bool visible)
{
	mMenu->SetVisible(visible);
}