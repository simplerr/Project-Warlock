#pragma once

#include "d3dUtil.h"

namespace GLib {
	class Texture2D;
	class Graphics;
}

class Player;

class HealthBar
{
public:
	HealthBar(float x, float y);
	~HealthBar();

	void Draw(GLib::Graphics* pGraphics);
	void OnResolutionChange();

	void SetFilled(float filled)	{mFilled = filled;}	// 0-1
	void Fill(float fill)			{mFilled + fill < 1.0f ? mFilled += fill : mFilled = 1.0f;}

	void SetPos(float x, float y)			{mPos = XMFLOAT2(x, y);}
	void SetWidth(int width)				{mWidth = width;}
	void SetHeight(int height)				{mHeight = height;}
	void SetSelectedPlayer(Player* pPlayer) {mSelectedPlayer = pPlayer;}

	float GetFilled()						{return mFilled;}
private:
	GLib::Texture2D* mBar;
	GLib::Texture2D* mBorder;
	Player*			 mSelectedPlayer;
	XMFLOAT2		 mPos;
	float			 mFilled;
	int mWidth;
	int mHeight;
};