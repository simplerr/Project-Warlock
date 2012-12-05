#pragma once
#include <map>
#include "Items.h"

using namespace std;

struct ItemKey
{
	ItemKey() {}
	ItemKey(ItemName name, int level) {
		this->name = name;
		this->level = level;
	}
	ItemName name;
	int level;
};

bool operator<(const ItemKey &l, const ItemKey &r);
bool operator==(const ItemKey &l, const ItemKey &r);

class ItemLoaderXML
{
public:
	ItemLoaderXML(string filename);
	~ItemLoaderXML();

	Item* GetItem(ItemKey key);
	ItemName StringToName(string name);
private:
	map<ItemKey, Item*>		mItemMap;
	map<string, int>		mStringKeyMap;	   
};