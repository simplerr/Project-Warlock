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

class Inventory : public ItemContainer
{
public:
	Inventory(int x, int y, int colums, float slotSize);
	~Inventory();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void AddItem(ItemName name, int level);
	void AddItem(BaseItem* pItem);
	void RemoveItem(ItemName name, int level);
	void UpdateItems();

	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(const ItemSlot& itemSlot);
	string GetHooverText(const BaseItem* pItem);

	void SetPlayer(Player* pPlayer);
private:
	Player*			 mPlayer;
};