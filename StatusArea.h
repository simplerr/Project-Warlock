#pragma once
#include "HudItemContainer.h"

class StatusArea : public HudItemContainer
{
public:
	StatusArea(float x, float y);
	~StatusArea();

	void OnHoover(const ItemSlot& itemSlot);
	void OnLeftPress(const ItemSlot& itemSlot);
	void OnRightPress(ItemSlot& itemSlot);
	string GetHooverText(HudItem* pItem);
	void AddItem(HudItem* pItem);
	void RemoveItem(HudItem* pItem, bool updateLayout = true) {};

	void RemoveStatusEffect(ItemKey itemKey);
private:

};