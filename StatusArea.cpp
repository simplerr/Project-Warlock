#include "StatusArea.h"

StatusArea::StatusArea(float x, float y)
	: ItemContainer(x, y, 5, 42)
{
	for(int i = 0; i < 6; i++)
		AddSlot();
}

StatusArea::~StatusArea()
{

}

void StatusArea::OnHoover(const ItemSlot& itemSlot)
{

}

void StatusArea::OnLeftPress(const ItemSlot& itemSlot)
{

}

void StatusArea::OnRightPress(ItemSlot& itemSlot)
{

}

string StatusArea::GetHooverText(BaseItem* pItem)
{
	return "Test string lolololo";
}

void StatusArea::AddItem(BaseItem* pItem)
{
	PlaceInFreeSlot(pItem);
}

void StatusArea::RemoveStatusEffect(ItemKey itemKey)
{
	auto itemSlots = GetItemSlots();

	vector<ItemSlot> newList;
	FreeAllSlots();
	for(int i = 0; i < itemSlots.size(); i++)
	{
		if(itemSlots[i].taken && itemSlots[i].item->GetName() != itemKey.name)
			PlaceInFreeSlot(itemSlots[i].item);
	}
}