#pragma once
#include "ItemContainer.h"

class StatusArea : public ItemContainer
{
public:
	StatusArea(float x, float y);
	~StatusArea();

	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(ItemSlot& itemSlot);
	string GetHooverText(BaseItem* pItem);
	void AddItem(BaseItem* pItem);

	void RemoveStatusEffect(ItemKey itemKey);
private:

};