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

	void AddItem(Client* pClient, ItemName name, int level);
	void RemoveItem(Client* pClient, ItemName name, int level);
	void UpdateItems();

	void OnHoover(const ItemSlot& item);
	void OnPress(const ItemSlot& item);

	void SetPlayer(Player* pPlayer);
private:
	Player*			 mPlayer;
	float mIconSize;
	XMFLOAT2		mPosition;
};