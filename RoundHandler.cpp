#include "RoundHandler.h"
#include "Graphics.h"

RoundHandler::RoundHandler()
{

}

RoundHandler::~RoundHandler()
{

}

void RoundHandler::Draw(GLib::Graphics* pGraphics)
{
	// Draw the scores.
	string scoreList;
	for(auto iter = mScoreMap.begin(); iter != mScoreMap.end(); iter++)
	{
		char score[10];
		sprintf(score, "%i", (*iter).second);
		scoreList += (*iter).first + ": " + score + "\n";
	}

	pGraphics->DrawText(scoreList, 900, 100, 14);

	char buffer[128];
	sprintf(buffer, "Timer: %.2f", mArenaState.elapsed);
	pGraphics->DrawText(buffer, 900, 200, 14);
}

ArenaState& RoundHandler::GetArenaState()
{
	return mArenaState;
}

void RoundHandler::SetArenaState(ArenaState state)
{
	mArenaState = state;
}

void RoundHandler::AddScore(string name, int score)
{
	if(mScoreMap.find(name) != mScoreMap.end())
		mScoreMap[name] += score;
	else
		mScoreMap[name] = 0;
}

void RoundHandler::SetWinner(string winner)
{
	mWinner = winner;
}

string RoundHandler::GetWinner()
{
	return mWinner;
}

void RoundHandler::SetCurrentState(CurrentState state)
{
	mArenaState.state = state;
}

map<string, int> RoundHandler::GetScoreMap()
{
	return mScoreMap;
}