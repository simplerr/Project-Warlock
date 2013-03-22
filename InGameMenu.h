#pragma once
#include "ControlManager.h"

class TextMenu;
class Label;

class InGameMenu : public ControlManager
{
public:
	InGameMenu(string luaScript);
	~InGameMenu();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void ButtonPressed(Label* pButton);
	void SetVisible(bool visible);
	bool GetChangeState();
private:
	TextMenu* mMenu;
	bool mChangeState;
};