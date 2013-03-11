#include "RoundHandler.h"
#include "Graphics.h"
#include "Camera.h"

RoundHandler::RoundHandler()
{
	mScoreBkgd = GLib::GetGraphics()->LoadTexture("textures/white_transparent.png");
}

RoundHandler::~RoundHandler()
{

}

void RoundHandler::Draw(GLib::Graphics* pGraphics)
{
	float width = 150;
	float height = 200;
	pGraphics->DrawScreenQuad(mScoreBkgd, GLib::GetClientWidth()-width/2, height/2, width, height);
	pGraphics->DrawText("Scores:", GLib::GetClientWidth()-width+20, 20, 25);

	// Draw the scores.
	string scoreList;
	for(auto iter = mScoreMap.begin(); iter != mScoreMap.end(); iter++)
	{
		char score[10];
		sprintf(score, "%i", (*iter).second);
		scoreList += (*iter).first + ": " + score + "\n";
	}

	pGraphics->DrawText(scoreList, GLib::GetClientWidth()-width+20, 50, 20);

	// Shopping timer.
	if(mArenaState.state == SHOPPING_STATE)
	{
		char buffer[256];
		sprintf(buffer, "Shopping time left: %.1f", mArenaState.length - mArenaState.elapsed);
		pGraphics->DrawText(buffer, GLib::GetClientWidth()/2-80, 30, 30, GLib::ColorRGBA(255, 0, 0, 255));
	}
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