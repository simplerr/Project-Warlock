#pragma once
#include "BitStream.h"
#include "d3dUtil.h"
#include "StatusEffect.h"
#include <string>

namespace GLib {
	class Graphics;
	class Input;
	class Texture2D;
	class StatusText;
}

class ItemLoaderXML;
class Inventory;
class Shop;
class Player;
class Client;
class SkillInventory;
class Chat;
class PlayerModule;
class StatusArea;
class HealthBar;
class GameOverOverlay;
class InGameMenu;
class AttributesUi;

/*
	--Item and Skill system breakdown--

	- Skill inherits from HudItem
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
	void OnResize(float width, float height);
	void DisplayGameOver(Client* pClient, string winner);
	void RemoveGameOverScreen();

	void HandleItemAdded(PlayerModule* pPlayer, RakNet::BitStream& bitstream);
	void HandleItemRemoved(PlayerModule* pPlayer, RakNet::BitStream& bitstream);
	void HandleChatMessage(RakNet::BitStream& bitstream);
	void HandleAddChatText(RakNet::BitStream& bitstream);
	void SetSelectedPlayer(PlayerModule* pPlayer);
	bool PointInsideUi(XMFLOAT3 position);
	void RemoveAllItems();

	void UpdateChatPosition();
	void SetStatusText(string text, float time, float size = 30, UINT32 color = GLib::ColorRGBA(0, 0, 0, 255));
	void OnMessageSent(string message);

	void OnStatusEffectAdded(ItemName type);
	void OnStatusEffectRemoved(ItemName type);

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void SetReady(bool ready);
	bool IsReady();
	bool HasChatFocus();

	Chat* GetChat();
	ItemLoaderXML* GetItemLoader();
private:
	ItemLoaderXML*				mItemLoader;
	Inventory*					mInventory;
	SkillInventory*				mSkillInventory;
	Shop*						mShop;
	Shop*						mSkillShop;
	GLib::Texture2D*			mBkgdTexture;
	GLib::Texture2D*			mWhiteTexture;
	GLib::StatusText*			mStatusText;
	StatusArea*					mStatusArea;
	Chat*						mChat;
	HealthBar*					mHealthBar;
	InGameMenu*					mInGameMenu;
	AttributesUi*				mAttributesUi;
	Client*						mClient;

	GameOverOverlay*			mGameOverOverlay;
	bool						mIsReady;	// to prevent mChat->MsgProc() when not loaded fully.
};