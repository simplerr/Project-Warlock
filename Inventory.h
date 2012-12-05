#pragma once
#include "Items.h"
#include "ItemContainer.h"
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
class Shop;

class Inventory : public ItemContainer
{
public:
	Inventory(int x, int y, int colums, float slotSize);
	~Inventory();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void AddItem(ItemName name, int level);
	void AddItem(BaseItem* pItem);
	void RemoveItem(BaseItem* pItem);
	void UpdateItems();

	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(ItemSlot& itemSlot);
	string GetHooverText(BaseItem* pItem);

	void SetPlayer(Player* pPlayer);
	void SetShop(Shop* pShop);
private:
	Player*		mPlayer;
	Shop*		mShop;

};