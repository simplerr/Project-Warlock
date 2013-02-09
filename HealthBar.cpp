#include "HealthBar.h"
#include "Graphics.h"
#include "Player.h"
#include "UiCoordinate.h"

HealthBar::HealthBar(float x, float y)
{
	SetPos(x, y);
	SetWidth(256);
	SetHeight(64);

	mBorder = GLib::GetGraphics()->LoadTexture("textures\\bar_border.bmp", 0x00000000);
	mBar = GLib::GetGraphics()->LoadTexture("textures\\bar.bmp");
	mFilled = 0.1f;
}

HealthBar::~HealthBar()
{

}

void HealthBar::Draw(GLib::Graphics* pGrahics)
{
	float current = mSelectedPlayer->GetCurrentHealth();
	float max = mSelectedPlayer->GetMaxHealth();
	SetFilled(current / max);

	pGrahics->DrawScreenQuad(mBorder, mPos.x, mPos.y, mWidth, mHeight);

	GLib::Rect sourceRect;
	sourceRect.left = 0.0f;
	sourceRect.right = mFilled * 32;	// 32 is image width
	sourceRect.top = 0.0f;
	sourceRect.bottom = 1.0f;

	pGrahics->DrawScreenQuad(mBar, mPos.x - (float)(mWidth-20)/2 + (float)(mWidth-20)*mFilled/2, mPos.y, (mWidth-20)*mFilled, mHeight-10);

	char buffer[128];
	sprintf(buffer, "%.1f/%.1f", current, max);
	pGrahics->DrawText(buffer, mPos.x - mWidth/2, mPos.y - mHeight/2 - 20, 20, GLib::ColorRGBA(255, 255, 255, 255));
}

void HealthBar::OnResolutionChange()
{
	UiCoordinate coord(UiAlignmentX::LEFT, BOTTOM, mPos.x, mPos.y, mWidth, mHeight, false, false);
	mPos = XMFLOAT2(coord.x, coord.y);
}