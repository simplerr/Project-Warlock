#pragma once
#include "Items.h"
#include "ItemLoaderXML.h"
#include "d3dUtil.h"
#include <vector>
using namespace std;

namespace GLib {
	class Graphics;
	class Input;
	struct Texture2D;
}

class ItemLoaderXML;
class Player;
class Client;

struct InventorySlot
{
	InventorySlot() {
		texture = nullptr;
		taken = false;
	}

	GLib::Texture2D* texture;
	Item item;
	bool taken;
};

class Inventory
{
public:
	Inventory();
	~Inventory();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void AddItem(Client* pClient, ItemName name, int level);
	void RemoveItem(Client* pClient, ItemName name, int level);
	void UpdateItems();
	void PlaceInFreeSlot(ItemKey itemKey);

	void SetItemLoader(ItemLoaderXML* pLoader);
	void SetPlayer(Player* pPlayer);
private:
	vector<InventorySlot> mInventorySlots;
	ItemLoaderXML*	mItemLoaderXML;
	GLib::Texture2D* mEmptySlotTexture;
	Player*			 mPlayer;
	float mIconSize;
	XMFLOAT2		mPosition;
};