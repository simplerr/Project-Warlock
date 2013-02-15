#pragma once
#include "d3dUtil.h"

namespace GLib {
	class Graphics;
	class Texture2D;
}
class Player;

class AttributesUi
{
public:
	AttributesUi(float x, float y);
	~AttributesUi();

	void Draw(GLib::Graphics* pGraphics);
	void SetSelectedPlayer(Player* pPlayer);
private:
	XMFLOAT2 mPosition;
	Player* mSelectedPlayer;;
	GLib::Texture2D* mWhiteTexture;
	string mDisplayText;
	float mWidth, mHeight;
};