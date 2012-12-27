#include "ItemContainer.h"
#include "Client.h"
#include "Input.h"
#include "Graphics.h"
#include "NetworkMessages.h"
#include "d3dUtil.h"
#include "D3DCore.h"
#include "UiCoordinate.h"

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
		XMFLOAT3 mousePos = pInput->MousePosition();
		/*mousePos.x *= GLib::GetGraphics()->GetD3D()->GetDimensionRatio().right;
		mousePos.y *= GLib::GetGraphics()->GetD3D()->GetDimensionRatio().bottom;*/

		if(mItemSlots[i].taken && InsideSlot(mItemSlots[i], mousePos))
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
			mItemSlots[i].item->DrawIcon(pGraphics, XMFLOAT2(pos.x, pos.y), mSlotSize);
			
			if(mHooveringSlotId == i) 
				pGraphics->DrawText(GetHooverText(mItemSlots[i].item), mPosition.x - mSlotSize/2, mPosition.y - mSlotSize * 1.2 * (mItemSlots.size() / mNumColums) / 2 - mSlotSize/2 - 49, 18);
		}	
		else {
			pGraphics->DrawScreenQuad(mEmptySlotTexture, pos.x, pos.y, mSlotSize, mSlotSize);

			//UiCoordinate coord(UiAlignmentX::CENTER, BOTTOM, pos.x, pos.y, mSlotSize, mSlotSize, false, false);
			//pGraphics->DrawScreenQuad(mEmptySlotTexture, coord.x, coord.y, coord.width, coord.height);
		}
	}

	pGraphics->DrawScreenQuad(mEmptySlotTexture, mPosition.x, mPosition.y, 20, 20);
}

void ItemContainer::AddSlot()
{
	ItemSlot newSlot;
	newSlot.position = mPosition;
	newSlot.position.x += mSlotSize * 1.2 * (mItemSlots.size() % mNumColums);
	newSlot.position.y += mSlotSize * 1.2 * (mItemSlots.size() / mNumColums);
	mItemSlots.push_back(newSlot);
	PerformLayout();
}

void ItemContainer::PerformLayout()
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		mItemSlots[i].position = mPosition; 
		mItemSlots[i].position.x -= mSlotSize * 1.2 * (mItemSlots.size() %  mNumColums) / 2;
		mItemSlots[i].position.y -= mSlotSize * 1.2 * (mItemSlots.size() / mNumColums) / 2;


		mItemSlots[i].position.x += mSlotSize * 1.2 * (i % mNumColums);
		mItemSlots[i].position.y += mSlotSize * 1.2 * (i / mNumColums);
	}
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

bool ItemContainer::HasFreeSlots()
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(!mItemSlots[i].taken)
			return true;
	}

	return false;
}

void ItemContainer::SendItemAdded(int playerId, ItemName itemName, int itemLevel)
{
	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(playerId);
	bitstream.Write(itemName);
	bitstream.Write(itemLevel);
	mClient->SendServerMessage(bitstream);
}


void ItemContainer::SendItemRemoved(int playerId, ItemName itemName, int itemLevel)
{
	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream.Write(playerId);
	bitstream.Write(itemName);
	bitstream.Write(itemLevel);
	mClient->SendServerMessage(bitstream);
}

void ItemContainer::SendGoldChange(int playerId, int newGold)
{
	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
	bitstream.Write(playerId);
	bitstream.Write(newGold);
	GetClient()->SendServerMessage(bitstream);
}

void ItemContainer::SetItemLoader(ItemLoaderXML* pLoader)
{
	mItemLoaderXML = pLoader;
}

void ItemContainer::SetPosition(float x, float y)
{
	mPosition = XMFLOAT2(x, y);
	PerformLayout();
}

ItemLoaderXML* ItemContainer::GetItemLoader()
{
	return mItemLoaderXML;
}

void ItemContainer::SetClient(Client* pClient)
{
	mClient = pClient;
}

Client*	ItemContainer::GetClient()
{
	return mClient;
}

ItemSlot* ItemContainer::GetItemSlot(ItemName name)
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(mItemSlots[i].taken && mItemSlots[i].item->GetName() == name)
			return &mItemSlots[i];
	}

	return nullptr;
}

XMFLOAT2 ItemContainer::GetPosition()
{
	return mPosition;
}

int	ItemContainer::GetHeightInSlots()
{
	return mItemSlots.size() / mNumColums;
}

void ItemContainer::OnResolutionChange()
{
	// GetHeightInSlots()*1.2 is probably wrong, should use some kind of spacing instead.
	UiCoordinate coord(UiAlignmentX::LEFT, BOTTOM, GetPosition().x, GetPosition().y, mSlotSize, mSlotSize*GetHeightInSlots()+GetHeightInSlots()*1.2, false, false);
	mPosition = XMFLOAT2(coord.x, coord.y);
	PerformLayout();
}