#pragma once
#include "BitStream.h"
#include "d3dUtil.h"
#include <string>

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
class Chat;

/*
	--Item and Skill system breakdown--

	- Skill inherits from BaseItem
	- ItemLoadeXML has a map of all Items, the skills are stored as Items in it as well
	- When an item is added to the skill inventory a Skill* is returned from Player::AddSkill() and used instead 
	  of the passed BaseItem*
*/

class UserInterface
{
public:
	UserInterface(Client* pClient);
	~UserInterface();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void HandleItemAdded(Player* pPlayer, RakNet::BitStream& bitstream);
	void HandleItemRemoved(Player* pPlayer, RakNet::BitStream& bitstream);
	void HandleChatMessage(RakNet::BitStream& bitstream);
	void HandleAddChatText(RakNet::BitStream& bitstream);
	void SetSelectedPlayer(Player* pPlayer);
	bool PointInsideUi(XMFLOAT3 position);

	void OnMessageSent(string message);

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	Chat* GetChat();
private:
	ItemLoaderXML*				mItemLoader;
	Inventory*					mInventory;
	SkillInventory*				mSkillInventory;
	Shop*						mShop;
	Shop*						mSkillShop;
	GLib::Texture2D*			mBkgdTexture;
	Chat*						mChat;
};