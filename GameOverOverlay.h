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

class GameOverOverlay
{
public:
	GameOverOverlay(float x, float y);
	~GameOverOverlay();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void SetScoreMap(map<string, int> scoreMap);
	void OnResize(float width, float height);

	void ButtonPressed(Button* pButton);
private:
	ControlManager*	mControlManager;
	TextMenu*		mScoreList;
	Button*		    mMainMenuButton;
};