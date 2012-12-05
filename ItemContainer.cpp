#include "ItemContainer.h"
#include "Client.h"
#include "Input.h"
#include "Graphics.h"

ItemContainer::ItemContainer(int x, int y, int colums, float slotSize)
{
	mPosition = XMFLOAT2(x, y);
	mNumColums = colums;
	mSlotSize = slotSize;
	mHooveringSlotId = -1;

	mEmptySlotTexture = GLib::GetGraphics()->LoadTexture("textures/icons/empty_slot.bmp");
}

ItemContainer::~ItemContainer()
{

}

void ItemContainer::Update(GLib::Input* pInput, float dt)
{
	mHooveringSlotId = -1;
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(mItemSlots[i].taken && InsideSlot(mItemSlots[i], pInput->MousePosition()))
		{
			OnHoover(mItemSlots[i]);
			if(pInput->KeyPressed(VK_LBUTTON))
				OnLeftPress(mItemSlots[i]);
			else if(pInput->KeyPressed(VK_RBUTTON))
				OnRightPress(mItemSlots[i]);

			mHooveringSlotId = i;
		}
	}
}

void ItemContainer::Draw(GLib::Graphics* pGraphics)
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		XMFLOAT2 pos = mItemSlots[i].position;
		if(mItemSlots[i].taken) {
			mItemSlots[i].item->Draw(pGraphics, XMFLOAT2(pos.x, pos.y), mSlotSize);
			
			if(mHooveringSlotId == i) 
				pGraphics->DrawText(GetHooverText(mItemSlots[i].item), mPosition.x - mSlotSize/2, mPosition.y - mSlotSize/2 - 49, 18);
		}	
		else
			pGraphics->DrawScreenQuad(mEmptySlotTexture, pos.x, pos.y, mSlotSize, mSlotSize);
	}
}

void ItemContainer::AddSlot()
{
	ItemSlot newSlot;
	newSlot.position = mPosition;
	newSlot.position.x += mSlotSize * 1.2 * (mItemSlots.size() % 3);
	newSlot.position.y += mSlotSize * 1.2 * (mItemSlots.size() / 3);
	mItemSlots.push_back(newSlot);
}

void ItemContainer::PlaceInFreeSlot(ItemKey itemKey)
{
	PlaceInFreeSlot(mItemLoaderXML->GetItem(itemKey));
}

void ItemContainer::PlaceInFreeSlot(BaseItem* pItem)
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(!mItemSlots[i].taken)
		{
			mItemSlots[i].item = pItem;
			mItemSlots[i].taken = true;
			break;
		}
	}
}

bool ItemContainer::InsideSlot(const ItemSlot& slot, XMFLOAT3 pos)
{
	RECT rect;
	rect.left = slot.position.x - mSlotSize/2;
	rect.right = slot.position.x + mSlotSize/2;
	rect.top = slot.position.y - mSlotSize/2;
	rect.bottom = slot.position.y + mSlotSize/2;

	return pos.x > rect.left && pos.x < rect.right && pos.y > rect.top && pos.y < rect.bottom;
}

void ItemContainer::FreeAllSlots()
{
	for(int i = 0; i < mItemSlots.size(); i++)
		mItemSlots[i].taken = false;
}

void ItemContainer::SetItemLoader(ItemLoaderXML* pLoader)
{
	mItemLoaderXML = pLoader;
}

ItemLoaderXML* ItemContainer::GetItemLoader()
{
	return mItemLoaderXML;
}

bool ItemContainer::HasFreeSlots()
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(!mItemSlots[i].taken)
			return true;
	}

	return false;
}

void ItemContainer::SetClient(Client* pClient)
{
	mClient = pClient;
}

Client*	ItemContainer::GetClient()
{
	return mClient;
}