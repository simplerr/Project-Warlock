#pragma once
#include <string>
#include <map>
#include "GameState.h"
#include "ServerData.h"
using namespace std;

class ControlManager;
class Label;
class Database;

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
	void OnResize(float width, float height);
protected:
	BrowsingState() {};
private:
	static BrowsingState mBrowsingState;
	Database*			 mDatabase;
	ControlManager*		 mControlManager;
	GLib::Texture2D*	 mBkgdTexture;
	map<string, ServerData>  mServerMap;
};