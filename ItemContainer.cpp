#include "ItemContainer.h"
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
			pGraphics->DrawScreenQuad(mItemSlots[i].texture, pos.x, pos.y, mSlotSize, mSlotSize);

			if(mHooveringSlotId == i) {
				pGraphics->DrawText(mItemSlots[i].item.description, mPosition.x - mSlotSize/2, mPosition.y - mSlotSize/2 - 25, 18);
			}
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
	Item item = mItemLoaderXML->GetItem(itemKey);
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(!mItemSlots[i].taken)
		{
			mItemSlots[i].item = item;
			mItemSlots[i].texture = GLib::GetGraphics()->LoadTexture(item.icon);
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