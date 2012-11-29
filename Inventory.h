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
	void AddItem(Item item);
	void RemoveItem(ItemName name, int level);
	void UpdateItems();

	void OnHoover(const ItemSlot& itemSlot);
	void OnPress(const ItemSlot& itemSlot);

	void SetPlayer(Player* pPlayer);
	void SetClient(Client* pClient);
private:
	Player*			 mPlayer;
	Client*			 mClient;
};