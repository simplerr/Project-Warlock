#pragma once
#include "BitStream.h"
#include "d3dUtil.h"

namespace GLib {
	class Graphics;
	class Input;
	class Texture2D;
}

class ItemLoaderXML;
class Inventory;
class Shop;
class Player;
class Client;
class SkillInventory;

class UserInterface
{
public:
	UserInterface(Client* pClient);
	~UserInterface();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void HandleItemAdded(Player* pPlayer, RakNet::BitStream& bitstream);
	void HandleItemRemoved(Player* pPlayer, RakNet::BitStream& bitstream);
	void SetSelectedPlayer(Player* pPlayer);
	bool PointInsideUi(XMFLOAT3 position);
private:
	ItemLoaderXML*				mItemLoader;
	Inventory*					mInventory;
	SkillInventory*				mSkillInventory;
	Shop*						mShop;
	Shop*						mSkillShop;
	GLib::Texture2D*			mBkgdTexture;
};