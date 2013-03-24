#include "HudItemContainer.h"
#include "Client.h"
#include "Input.h"
#include "Graphics.h"
#include "NetworkMessages.h"
#include "d3dUtil.h"
#include "D3DCore.h"
#include "UiCoordinate.h"

HudItemContainer::HudItemContainer(int x, int y, int colums, float slotSize)
{
	SetHooverInfoPos(XMFLOAT2(0, 0));
	SetPosition(x, y);
	SetHooverInfoBelow(false);
	SetDrawEmptySlots(true);
	mHooverBkgd = nullptr;
	mNumColums = colums;
	mSlotSize = slotSize;
	mHooveringSlotId = -1;

	mEmptySlotTexture = GLib::GetGraphics()->LoadTexture("data/textures/icons/empty_slot.bmp");
}

HudItemContainer::~HudItemContainer()
{

}

void HudItemContainer::Update(GLib::Input* pInput, float dt)
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

void HudItemContainer::Draw(GLib::Graphics* pGraphics)
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		XMFLOAT2 pos = mItemSlots[i].position;
		if(mItemSlots[i].taken)
			mItemSlots[i].item->DrawIcon(pGraphics, XMFLOAT2(pos.x, pos.y), mSlotSize);
		else
		{
			if(mDrawEmptySlots)
				pGraphics->DrawScreenQuad(mEmptySlotTexture, pos.x, pos.y, mSlotSize, mSlotSize);

			//UiCoordinate coord(UiAlignmentX::CENTER, BOTTOM, pos.x, pos.y, mSlotSize, mSlotSize, false, false);
			//pGraphics->DrawScreenQuad(mEmptySlotTexture, coord.x, coord.y, coord.width, coord.height);
		}
	}	

	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(mHooveringSlotId == i) 
		{
			string name = mItemSlots[i].item->GetAttributes().name;
			string description = GetHooverText(mItemSlots[i].item);

			// * represents newline(\n).
			std::replace(description.begin(), description.end(), '*', '\n');

			GLib::Rect nameRect = pGraphics->MeasureText(name, 25, "Arial");
			GLib::Rect descRect = pGraphics->MeasureText(description, 18, "Arial");
			float scale = 1.1f;

			float rectHeight = (nameRect.bottom/2 + descRect.bottom/2) * scale;

			// mHooverInfoPos is the origin for the hoover box, probably top left or bottom left.
			float y = GetPosition().y + mHooverInfoPos.y + (mHooverInfoBelow ? 1 : -1) * rectHeight;

			if(mHooverBkgd != nullptr) {
					
				pGraphics->DrawScreenQuad(mHooverBkgd, GetPosition().x + mHooverInfoPos.x + descRect.right/2, y, descRect.right * scale, (descRect.bottom + nameRect.bottom)* scale);
			}

			float text_y = y - rectHeight/scale*(((scale-1.0f)/2)+1);
			pGraphics->DrawText(name, mPosition.x + mHooverInfoPos.x, text_y, 25, GLib::ColorRGBA(204, 102, 34, 255));
			pGraphics->DrawText(description, mPosition.x + mHooverInfoPos.x, text_y + nameRect.bottom, 18);
		}
	}
}

void HudItemContainer::AddSlot()
{
	ItemSlot newSlot;
	newSlot.position = mPosition;
	newSlot.position.x += mSlotSize * 1.2 * (mItemSlots.size() % mNumColums);
	newSlot.position.y += mSlotSize * 1.2 * (mItemSlots.size() / mNumColums);
	mItemSlots.push_back(newSlot);
	PerformLayout();
}

void HudItemContainer::PerformLayout()
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

void HudItemContainer::PlaceInFreeSlot(ItemKey itemKey)
{
	PlaceInFreeSlot(mItemLoaderXML->GetItem(itemKey));
}

void HudItemContainer::PlaceInFreeSlot(HudItem* pItem)
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

bool HudItemContainer::InsideSlot(const ItemSlot& slot, XMFLOAT3 pos)
{
	RECT rect;
	rect.left = slot.position.x - mSlotSize/2;
	rect.right = slot.position.x + mSlotSize/2;
	rect.top = slot.position.y - mSlotSize/2;
	rect.bottom = slot.position.y + mSlotSize/2;

	return pos.x > rect.left && pos.x < rect.right && pos.y > rect.top && pos.y < rect.bottom;
}

void HudItemContainer::FreeAllSlots()
{
	for(int i = 0; i < mItemSlots.size(); i++)
		mItemSlots[i].taken = false;
}

bool HudItemContainer::HasFreeSlots()
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(!mItemSlots[i].taken)
			return true;
	}

	return false;
}

void HudItemContainer::SendItemAdded(int playerId, ItemName itemName, int itemLevel)
{
	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_ADDED);
	bitstream.Write(playerId);
	bitstream.Write(itemName);
	bitstream.Write(itemLevel);
	mClient->SendServerMessage(bitstream);
}

void HudItemContainer::SendItemRemoved(int playerId, ItemName itemName, int itemLevel)
{
	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_ITEM_REMOVED);
	bitstream.Write(playerId);
	bitstream.Write(itemName);
	bitstream.Write(itemLevel);
	mClient->SendServerMessage(bitstream);
}

void HudItemContainer::SendGoldChange(int playerId, int newGold)
{
	// Send to server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_GOLD_CHANGE);
	bitstream.Write(playerId);
	bitstream.Write(newGold);
	GetClient()->SendServerMessage(bitstream);
}

void HudItemContainer::RemoveAllItems()
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		Item* item = mItemLoaderXML->GetItem(ItemKey(mItemSlots[i].item->GetName(), mItemSlots[i].item->GetLevel()));
		if(item != nullptr)
			RemoveItem(item, false);
	}
}

void HudItemContainer::SetItemLoader(ItemLoaderXML* pLoader)
{
	mItemLoaderXML = pLoader;
}

void HudItemContainer::SetPosition(float x, float y)
{
	mPosition = XMFLOAT2(x, y);
	PerformLayout();
}

ItemLoaderXML* HudItemContainer::GetItemLoader()
{
	return mItemLoaderXML;
}

void HudItemContainer::SetClient(Client* pClient)
{
	mClient = pClient;
}

Client*	HudItemContainer::GetClient()
{
	return mClient;
}

ItemSlot* HudItemContainer::GetItemSlot(ItemName name)
{
	for(int i = 0; i < mItemSlots.size(); i++)
	{
		if(mItemSlots[i].taken && mItemSlots[i].item->GetName() == name)
			return &mItemSlots[i];
	}

	return nullptr;
}

XMFLOAT2 HudItemContainer::GetPosition()
{
	return mPosition;
}

int	HudItemContainer::GetHeightInSlots()
{
	return mItemSlots.size() / mNumColums;
}

void HudItemContainer::OnResolutionChange()
{
	// GetHeightInSlots()*1.2 is probably wrong, should use some kind of spacing instead.
	UiCoordinate coord(UiAlignmentX::LEFT, BOTTOM, GetPosition().x, GetPosition().y, mSlotSize, mSlotSize*GetHeightInSlots()+GetHeightInSlots()*1.2, false, false);
	mPosition = XMFLOAT2(coord.x, coord.y);
	PerformLayout();
}

vector<ItemSlot> HudItemContainer::GetItemSlots()
{
	return mItemSlots;
}

void HudItemContainer::SetHooverInfoPos(XMFLOAT2 pos)
{
	mHooverInfoPos = pos;
}

void HudItemContainer::SetHooverBkgd(string texture)
{
	mHooverBkgd = GLib::GetGraphics()->LoadTexture(texture);
}

void HudItemContainer::SetHooverInfoBelow(bool below)
{
	mHooverInfoBelow = below;
}

void HudItemContainer::SetDrawEmptySlots(bool drawEmpty)
{
	mDrawEmptySlots = drawEmpty;
}

float HudItemContainer::GetWidth()
{
	return mNumColums * mSlotSize * 1.2;
}