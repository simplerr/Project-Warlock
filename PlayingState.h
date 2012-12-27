#pragma once
#include "GameState.h"

class ControlManager;
class Client;
class Database;

class PlayingState : public GameState
{
public:
	void Init(Game* pGame);
	void Cleanup(void);

	void Pause();
	void Resume();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(GLib::Input* pInput, double dt);
	void Draw(GLib::Graphics* pGraphics);
	void OnResize(float width, float height);

	static PlayingState* Instance() {
		return &mPlayingState;
	}

	void SetDatabase(Database* pDatabase);
	void SetClient(Client* pClient);
	Client* GetClient();
protected:
	PlayingState() {};
private:
	static PlayingState mPlayingState;

	Client*			mClient;
	ControlManager* mControlManager;
};