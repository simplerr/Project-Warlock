#pragma once
#include "GameState.h"
#include "ServerData.h"

class ControlManager;
class Client;
class Database;
class Button;

class LobbyState : public GameState
{
public:
	void Init(Game* pGame);
	void Cleanup(void);

	void Pause();
	void Resume();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(GLib::Input* pInput, double dt);
	void Draw(GLib::Graphics* pGraphics);

	static LobbyState* Instance() {
		return &mLobbyState;
	}

	void BuildUi();
	void SetServerData(ServerData data);
	void ButtonPressed(Button* pButton);
	void StartGame();
	void OnResize(float width, float height);
protected:
	LobbyState() {};
private:
	static LobbyState mLobbyState;

	ControlManager* mControlManager;
	Client*			mClient;
	ServerData		mServerData;
	Database*		mDatabase;
	GLib::Texture2D* mBkgdTexture;
	GLib::Texture2D* mPlayerListBkgd;
	bool			mIsHost;
};