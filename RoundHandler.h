#pragma once
#include "States.h"
#include <map>
#include <string>
using namespace std;

namespace GLib {
	class Graphics;
}

class RoundHandler
{
public:
	RoundHandler();
	~RoundHandler();

	void Draw(GLib::Graphics* pGraphics);
	
	void AddScore(string name, int score);
	void SetArenaState(ArenaState state);
	void SetCurrentState(CurrentState state);
	void SetWinner(string winner);

	ArenaState& GetArenaState();
	string GetWinner();
private:
	// These relate to each other, RoundHandler or similar
	ArenaState			mArenaState;
	map<string, int>	mScoreMap;
	string				mWinner;
};