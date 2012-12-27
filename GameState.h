#pragma once
#include "Game.h"

namespace GLib {
	class Graphics;
	class Input;
}

// Reference: http://gamedevgeek.com/tutorials/managing-game-states-in-c/

//! Abstract baseclass.
class GameState
{
public:
	virtual void Init(Game* pGame) = 0;
	virtual void Cleanup(void) = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Update(GLib::Input* pInput, double dt) = 0;
	virtual void Draw(GLib::Graphics* pGraphics) = 0;
	virtual void OnResize(float width, float height) {};

	void SetGame(Game* game) {
		mGame = game;
	}
	void ChangeState(GameState* state) {
		mGame->ChangeState(state);
	}
protected:
	GameState(){};
private:
	Game *mGame;
};	// Class

extern GameState* gGameState;