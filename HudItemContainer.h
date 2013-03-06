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

	HudItem*	item;
	XMFLOAT2	position;
	bool		taken;
};

/************************************************************************
ItemName is used because it has to be sent over the network to other clients
/************************************************************************/
class HudItemContainer
{
public:
	HudItemContainer(int x, int y, int colums, float slotSize);
	~HudItemContainer();

	virtual void Update(GLib::Input* pInput, float dt);
	virtual void Draw(GLib::Graphics* pGraphics);
	virtual void OnHoover(const ItemSlot& itemSlot) = 0;
	virtual void OnLeftPress(const ItemSlot& itemSlot) = 0;
	virtual void OnRightPress(ItemSlot& itemSlot) = 0;
	virtual string GetHooverText(HudItem* pItem) = 0;
	virtual void AddItem(HudItem* pItem) = 0;

	void AddSlot();
	void PlaceInFreeSlot(HudItem* pItem);
	void PlaceInFreeSlot(ItemKey itemKey);
	void FreeAllSlots();
	void PerformLayout();
	void OnResolutionChange();

	void SendItemAdded(int playerId, ItemName itemName, int itemLevel);
	void SendItemRemoved(int playerId, ItemName itemName, int itemLevel);
	void SendGoldChange(int playerId, int newGold);

	void SetItemLoader(ItemLoaderXML* pLoader);
	void SetClient(Client* pClient);
	void SetPosition(float x, float y);
	void SetHooverInfoPos(XMFLOAT2 pos);
	void SetHooverBkgd(string texture);
	void SetHooverInfoBelow(bool below);
	void SetDrawEmptySlots(bool drawEmpty);
	
	bool HasFreeSlots();
	bool InsideSlot(const ItemSlot& slot, XMFLOAT3 pos);
	bool IsLocalPlayerSelected();
	ItemLoaderXML*	GetItemLoader();
	Client*			GetClient();
	ItemSlot*		GetItemSlot(ItemName name);
	XMFLOAT2		GetPosition();
	int				GetHeightInSlots();
	vector<ItemSlot> GetItemSlots();
	float			GetWidth();
private:
	vector<ItemSlot>	mItemSlots;
	ItemLoaderXML*		mItemLoaderXML;
	GLib::Texture2D*	mEmptySlotTexture;
	GLib::Texture2D*	mHooverBkgd;
	Client*				mClient;
	XMFLOAT2			mPosition;
	XMFLOAT2			mHooverInfoPos;
	bool				mHooverInfoBelow;
	bool				mDrawEmptySlots;
	float				mSlotSize;
	int					mNumColums;
	int					mHooveringSlotId;
};