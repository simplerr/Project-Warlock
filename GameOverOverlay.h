#pragma once
#include <map>
#include <string>
#include "d3dUtil.h"
using namespace std;

namespace GLib {
	class Graphics;
	class Input;
}

class ControlManager;
class TextMenu;
class Button;
class UserInterface;

class GameOverOverlay
{
public:
	GameOverOverlay(float x, float y);
	~GameOverOverlay();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void SetScoreMap(map<string, int> scoreMap);
	void SetUserInterface(UserInterface* pInterface);
	void OnResize(float width, float height);
	bool GetChangeState();

	void ButtonPressed(Button* pButton);
private:
	ControlManager*	mControlManager;
	UserInterface*	mUserInterface; // Used to inform if the state was changed.
	TextMenu*		mScoreList;
	Button*		    mMainMenuButton;
	bool			mChangeState;
};