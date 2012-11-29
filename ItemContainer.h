#pragma once
#include "Items.h"
#include "ItemLoaderXML.h"
#include "d3dUtil.h"
#include <vector>

namespace GLib {
	class Graphics;
	class Input;
	struct Texture2D;
}

class ItemLoaderXML;

struct ItemSlot
{
	ItemSlot() {
		texture = nullptr;
		taken = false;
	}

	GLib::Texture2D* texture;
	Item item;
	XMFLOAT2 position;
	bool taken;
};

class ItemContainer
{
public:
	ItemContainer(int x, int y, int colums, float slotSize);
	~ItemContainer();

	virtual void Update(GLib::Input* pInput, float dt);
	virtual void Draw(GLib::Graphics* pGraphics);
	virtual void OnHoover(const ItemSlot& itemSlot) = 0;
	virtual void OnPress(const ItemSlot& itemSlot) = 0;

	void AddSlot();
	void PlaceInFreeSlot(ItemKey itemKey);
	void FreeAllSlots();
	void SetItemLoader(ItemLoaderXML* pLoader);
	
	bool InsideSlot(const ItemSlot& slot, XMFLOAT3 pos);
	ItemLoaderXML* GetItemLoader();
private:
	vector<ItemSlot>	mItemSlots;
	ItemLoaderXML*		mItemLoaderXML;
	GLib::Texture2D*	mEmptySlotTexture;
	XMFLOAT2			mPosition;
	float				mSlotSize;
	int					mNumColums;

};