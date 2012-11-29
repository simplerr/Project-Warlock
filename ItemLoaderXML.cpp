#include "ItemLoaderXML.h"
#include "Items.h"
#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"

bool operator<(const ItemKey &l, const ItemKey &r) 
{ 
	float leftCount = 1000 * l.name + l.level;
	float rightCount = 1000 * r.name + r.level;

	return leftCount < rightCount;

	if(l.name < r.name && l.level < r.level)
		return true;
	else
		return false;
}

bool operator==(const ItemKey &l, const ItemKey &r)
{
	return l.name == r.name && l.level == r.level;
}

ItemLoaderXML::ItemLoaderXML(string filename)
{
	// Setup the string to ItemKey map.
	mStringKeyMap["IRON_ARMOR"] = IRON_ARMOR;
	mStringKeyMap["REGEN_CAP"] = REGEN_CAP;

	// Load all models from the XML file.
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();

	// Get the root element.
	TiXmlElement* root = doc.FirstChildElement();

	// Load items from the file.
	for(TiXmlElement* item = root->FirstChildElement("Item"); item != NULL; item = item->NextSiblingElement("Item"))
	{
		string name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		string desc = item->FirstChildElement("Desc")->GetText();
		string icon = item->Attribute("icon") == NULL ? "#NOVALUE" : item->Attribute("icon");

		// Loop over all levels.
		for(TiXmlElement* level = item->FirstChildElement("Level"); level != NULL; level = level->NextSiblingElement("Level"))
		{
			Item loadedItem;
			loadedItem.name = name;
			loadedItem.description = desc;
			loadedItem.icon = icon;
			loadedItem.level = atoi(level->Attribute("num"));

			loadedItem.health = level->Attribute("health") == NULL ? 0.0f : atof(level->Attribute("health"));
			loadedItem.regen = level->Attribute("regen") == NULL ? 0.0f : atof(level->Attribute("regen"));
			loadedItem.movementSpeed = level->Attribute("movementSpeed") == NULL ? 0.0f : atof(level->Attribute("movementSpeed"));
			loadedItem.knockbakResistance = level->Attribute("knockbakResistance") == NULL ? 0.0f : atof(level->Attribute("knockbakResistance"));
			loadedItem.lavaImmunity = level->Attribute("lavaImmunity") == NULL ? 0.0f : atof(level->Attribute("lavaImmunity"));
			loadedItem.damage = level->Attribute("damage") == NULL ? 0.0f : atof(level->Attribute("damage"));
			loadedItem.lifesteal = level->Attribute("lifesteal") == NULL ? 0.0f : atof(level->Attribute("lifesteal"));
			loadedItem.price = level->Attribute("price") == NULL ? 10.0f : atoi(level->Attribute("price"));

			ItemKey key;
			key.name = (ItemName)mStringKeyMap[loadedItem.name];
			key.level = loadedItem.level;

			mItemMap[key] = loadedItem;
		}
	}
}

ItemLoaderXML::~ItemLoaderXML()
{

}

Item ItemLoaderXML::GetItem(ItemKey key)
{
	return mItemMap[key];
}

ItemName ItemLoaderXML::StringToName(string name)
{
	return (ItemName)mStringKeyMap[name];
}