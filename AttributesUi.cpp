#include "AttributesUi.h"
#include "Player.h"
#include "Graphics.h"

char attributes1[1024];
char attributes2[1024];

AttributesUi::AttributesUi(float x, float y)
{
	mPosition = XMFLOAT2(x, y);
	mSelectedPlayer = nullptr;
	mWhiteTexture = GLib::GetGraphics()->LoadTexture("data/textures/white_transparent.png");

	mWidth = 300;
	mHeight = 120;
}

AttributesUi::~AttributesUi()
{

}

void AttributesUi::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mWhiteTexture, mPosition.x, GLib::GetClientHeight() - mHeight/2*1.6, mWidth, mHeight);

	if(mSelectedPlayer != nullptr)
	{
		float left = mPosition.x - mWidth/2;
		float top = GLib::GetClientHeight() - mHeight - 25;
		sprintf(attributes1, "Movement Speed: %.1f\nRegen: %.1f\nKnockback resistance: %.1f\nLava immunity: %.1f\nLifesteal: %.1f\n", mSelectedPlayer->GetMovementSpeed()*100,
			mSelectedPlayer->GetRegen(), mSelectedPlayer->GetKnockBackResistance(), mSelectedPlayer->GetLavaImmunity(), mSelectedPlayer->GetLifeSteal());

		pGraphics->DrawText(attributes1, left+20, top, 15);

		// Gold display (to the right).
		char buffer[128];
		sprintf(buffer, "Gold: %i", mSelectedPlayer->GetGold());

		pGraphics->DrawScreenQuad(mWhiteTexture, GLib::GetClientWidth()-50, GLib::GetClientHeight()-200-20, 100, 40);
		pGraphics->DrawText(buffer, GLib::GetClientWidth()-50-40, GLib::GetClientHeight()-200-20-10, 20);
	}
}

void AttributesUi::SetSelectedPlayer(Player* pPlayer)
{
	mSelectedPlayer = pPlayer;
}