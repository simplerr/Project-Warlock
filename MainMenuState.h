#pragma once
#include "GameState.h"

class ControlManager;
class Label;
class TextMenu;

class MainMenuState : public GameState
{
public:
	void Init(Game* pGame);
	void Cleanup(void);

	void Pause();
	void Resume();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(GLib::Input* pInput, double dt);
	void Draw(GLib::Graphics* pGraphics);

	static MainMenuState* Instance() {
		return &mMainMenuState;
	}

	void BuildUi();
	void OnResize(float width, float height);
	void OnMenuItemPressed(Label* pLabel);
protected:
	MainMenuState() {};
private:
	static MainMenuState mMainMenuState;
	ControlManager*		 mControlManager;
	GLib::Texture2D*	 mBkgdTexture;
	TextMenu*			 mMenu;
	string				 mChageToState;
};