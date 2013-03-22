#pragma once
#include "GameState.h"

class ControlManager;
class Label;
class Button;

class OptionsState : public GameState
{
public:
	void Init(Game* pGame);
	void Cleanup(void);

	void Pause();
	void Resume();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(GLib::Input* pInput, double dt);
	void Draw(GLib::Graphics* pGraphics);

	static OptionsState* Instance() {
		return &mOptionsState;
	}

	void BuildUi();
	void OnResize(float width, float height);
	void OnButtonPressed(Button* pButton);
protected:
	OptionsState() {};
private:
	static OptionsState	 mOptionsState;
	ControlManager*		 mControlManager;
	GLib::Texture2D*	 mBkgdTexture;
	GLib::Texture2D*	 mWhiteTexture;
	Button*				 mBackButton;
	HWND				 mhNameBox;
	HWND				 mhServerNameBox;
	HWND				 mhLookSensBox;
	string				 mChangeState;
};