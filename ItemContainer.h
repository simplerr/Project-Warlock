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
class Client;

struct ItemSlot
{
	ItemSlot() {
		taken = false;
	}

	BaseItem*	item;
	XMFLOAT2	position;
	bool		taken;
};

class ItemContainer
{
public:
	ItemContainer(int x, int y, int colums, float slotSize);
	~ItemContainer();

	virtual void Update(GLib::Input* pInput, float dt);
	virtual void Draw(GLib::Graphics* pGraphics);
	virtual void OnHoover(const ItemSlot& itemSlot) = 0;
	virtual void OnLeftPress(const ItemSlot& itemSlot) = 0;
	virtual void OnRightPress(const ItemSlot& itemSlot) = 0;
	virtual string GetHooverText(BaseItem* pItem) = 0;
	virtual void AddItem(BaseItem* pItem) = 0;

	void AddSlot();
	void PlaceInFreeSlot(BaseItem* pItem);
	void PlaceInFreeSlot(ItemKey itemKey);
	void FreeAllSlots();
	void SetItemLoader(ItemLoaderXML* pLoader);
	void SetClient(Client* pClient);
	
	bool HasFreeSlots();
	bool InsideSlot(const ItemSlot& slot, XMFLOAT3 pos);
	bool IsLocalPlayerSelected();
	ItemLoaderXML* GetItemLoader();
	Client*	GetClient();
private:
	vector<ItemSlot>	mItemSlots;
	ItemLoaderXML*		mItemLoaderXML;
	GLib::Texture2D*	mEmptySlotTexture;
	Client*				mClient;
	XMFLOAT2			mPosition;
	float				mSlotSize;
	int					mNumColums;
	int					mHooveringSlotId;

};