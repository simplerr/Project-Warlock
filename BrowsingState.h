#pragma once
#include <string>
#include <map>
#include "GameState.h"
#include "ServerData.h"
using namespace std;

class ControlManager;
class Label;
class Database;
class Button;
class TextMenu;

class BrowsingState : public GameState
{
public:
	void Init(Game* pGame);
	void Cleanup(void);

	void Pause();
	void Resume();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(GLib::Input* pInput, double dt);
	void Draw(GLib::Graphics* pGraphics);

	static BrowsingState* Instance() {
		return &mBrowsingState;
	}

	void BuildUi();
	void OnServerPressed(Label* pLabel);
	void OnButtonPressed(Button* pButton);
	void OnResize(float width, float height);

	void RefreshServerList();
protected:
	BrowsingState() {};
private:
	static BrowsingState mBrowsingState;
	Database*			 mDatabase;
	ControlManager*		 mControlManager;
	TextMenu*			 mServerMenu;
	GLib::Texture2D*	 mBkgdTexture;
	map<string, ServerData>  mServerMap;
	string				 mSelectedServerHost;
	bool				 mBackToMenu;
};