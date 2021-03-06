#include "ItemLoaderXML.h"
#include "Items.h"
#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "StatusEffect.h"

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
	// Load all models from the XML file.
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();

	// Get the root element.
	TiXmlElement* root = doc.FirstChildElement();

	SetupKeyMap();

	LoadItems(root);
	LoadSkills(root);
	LoadStatusEffects(root);

	mFilename = filename;
}

ItemLoaderXML::~ItemLoaderXML()
{
	for(auto iter = mItemMap.begin(); iter != mItemMap.end(); iter++)
		delete (*iter).second;

	mItemMap.clear();
}

void ItemLoaderXML::SetupKeyMap()
{
	// Setup the string to ItemKey map.
	mStringKeyMap["Iron Armor"] = IRON_ARMOR;
	mStringKeyMap["Regen Cap"] = REGEN_CAP;
	mStringKeyMap["Boots of speed"] = BOOTS_OF_SPEED;
	mStringKeyMap["Mask of madness"] = MASK_OF_MADNESS;
	mStringKeyMap["Knockback shield"] = KNOCKBACK_SHIELD;
	mStringKeyMap["Lava armor"] = LAVA_ARMOR;
	mStringKeyMap["Magic wand"] = MAGIC_WAND;

	// Skills
	mStringKeyMap["Fireball"] = SKILL_FIREBALL;
	mStringKeyMap["Hook"] = SKILL_HOOK;
	mStringKeyMap["Frostnova"] = SKILL_FROSTNOVA;
	mStringKeyMap["Teleport"] = SKILL_TELEPORT;	
	mStringKeyMap["Meteor"] = SKILL_METEOR;
	mStringKeyMap["Venom Acid"] = SKILL_VENOM;
	mStringKeyMap["Grappling hook"] = SKILL_GRAPPLING_HOOK;

	// Status effects.
	mStringKeyMap["Freeze Effect"] = FREEZE_STATUS;	
	mStringKeyMap["Teleport Effect"] = TELEPORT_STATUS;
	mStringKeyMap["Poisoned Effect"] = POISON_STATUS;
}

/*void ItemLoaderXML::ReloadItems()
{
	for(auto iter = mItemMap.begin(); iter != mItemMap.end(); iter++)
		delete (*iter).second;

	mItemMap.clear();

	TiXmlDocument doc(mFilename.c_str());
	doc.LoadFile();

	// Get the root element.
	TiXmlElement* root = doc.FirstChildElement();

	LoadItems(root);
	LoadSkills(root);
	LoadStatusEffects(root);
}*/

void ItemLoaderXML::LoadItems(TiXmlElement* pRoot)
{
	TiXmlElement* items = pRoot->FirstChildElement("Items");
	for(TiXmlElement* item = items->FirstChildElement("Item"); item != NULL; item = item->NextSiblingElement("Item"))
	{
		string name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		string desc = item->FirstChildElement("Desc")->GetText();
		string icon = item->Attribute("icon") == NULL ? "#NOVALUE" : item->Attribute("icon");

		// Loop over all levels.
		for(TiXmlElement* level = item->FirstChildElement("Level"); level != NULL; level = level->NextSiblingElement("Level"))
		{
			// Load the attributes.
			Attributes attributes;
			attributes.health				= level->Attribute("health") == NULL ? 0.0f : atof(level->Attribute("health"));
			attributes.regen				= level->Attribute("regen") == NULL ? 0.0f : atof(level->Attribute("regen"));
			attributes.movementSpeed		= level->Attribute("movementSpeed") == NULL ? 0.0f : atof(level->Attribute("movementSpeed"));
			attributes.knockbackResistance	= level->Attribute("knockbakResistance") == NULL ? 0.0f : atof(level->Attribute("knockbakResistance"));
			attributes.lavaImmunity			= level->Attribute("lavaImmunity") == NULL ? 0.0f : atof(level->Attribute("lavaImmunity"));
			attributes.damage				= level->Attribute("damage") == NULL ? 0.0f : atof(level->Attribute("damage"));
			attributes.lifesteal			= level->Attribute("lifesteal") == NULL ? 0.0f : atof(level->Attribute("lifesteal"));
			attributes.cooldown				= level->Attribute("cooldown") == NULL ? 0.0f : atof(level->Attribute("cooldown"));
			attributes.dot					= level->Attribute("dot") == NULL ? 0.0f : atof(level->Attribute("dot"));
			attributes.slow					= level->Attribute("slow") == NULL ? 0.0f : atof(level->Attribute("slow"));
			attributes.duration				= level->Attribute("duration") == NULL ? 0.0f : atof(level->Attribute("duration"));
			attributes.name					= name;

			// Set the attributes and the other data.
			Item* loadedItem = new Item(icon);
			loadedItem->SetAttributes(attributes);
			loadedItem->SetCost(level->Attribute("cost") == NULL ? 10.0f : atoi(level->Attribute("cost")));
			loadedItem->SetSkill(false);
			loadedItem->SetName(StringToName(name));
			loadedItem->SetDescription(desc);
			loadedItem->SetLevel(atoi(level->Attribute("num")));

			// Create map key.
			ItemKey key;
			key.name = (ItemName)mStringKeyMap[name];
			key.level = loadedItem->GetLevel();

			// Add to map.
			mItemMap[key] = loadedItem;
		}
	}
}

void ItemLoaderXML::LoadSkills(TiXmlElement* pRoot)
{
	TiXmlElement* skills = pRoot->FirstChildElement("Skills");
	for(TiXmlElement* skill = skills->FirstChildElement("Skill"); skill != NULL; skill = skill->NextSiblingElement("Skill"))
	{
		string name = skill->Attribute("name") == NULL ? "#NOVALUE" : skill->Attribute("name");
		string desc = skill->FirstChildElement("Desc")->GetText();
		string icon = skill->Attribute("icon") == NULL ? "#NOVALUE" : skill->Attribute("icon");
		string hotkey = skill->Attribute("hotkey") == NULL ? " " : skill->Attribute("hotkey");

		// Loop over all levels.
		for(TiXmlElement* level = skill->FirstChildElement("Level"); level != NULL; level = level->NextSiblingElement("Level"))
		{
			// Load the attributes.
			Attributes attributes;
			attributes.health				= level->Attribute("health") == NULL ? 0.0f : atof(level->Attribute("health"));
			attributes.regen				= level->Attribute("regen") == NULL ? 0.0f : atof(level->Attribute("regen"));
			attributes.movementSpeed		= level->Attribute("movementSpeed") == NULL ? 0.0f : atof(level->Attribute("movementSpeed"));
			attributes.knockbackResistance	= level->Attribute("knockbakResistance") == NULL ? 0.0f : atof(level->Attribute("knockbakResistance"));
			attributes.lavaImmunity			= level->Attribute("lavaImmunity") == NULL ? 0.0f : atof(level->Attribute("lavaImmunity"));
			attributes.damage				= level->Attribute("damage") == NULL ? 0.0f : atof(level->Attribute("damage"));
			attributes.lifesteal			= level->Attribute("lifesteal") == NULL ? 0.0f : atof(level->Attribute("lifesteal"));
			attributes.cooldown				= level->Attribute("cooldown") == NULL ? 0.0f : atof(level->Attribute("cooldown"));
			attributes.dot					= level->Attribute("dot") == NULL ? 0.0f : atof(level->Attribute("dot"));
			attributes.slow					= level->Attribute("slow") == NULL ? 0.0f : atof(level->Attribute("slow"));
			attributes.duration				= level->Attribute("duration") == NULL ? 0.0f : atof(level->Attribute("duration"));
			attributes.hotkey				= hotkey;
			attributes.name					= name;

			// Set the attributes and the other data.
			Item* loadedItem = new Item(icon);
			loadedItem->SetAttributes(attributes);
			loadedItem->SetCost(level->Attribute("cost") == NULL ? 10.0f : atoi(level->Attribute("cost")));
			loadedItem->SetSkill(true);
			loadedItem->SetName(StringToName(name));
			loadedItem->SetDescription(desc);
			loadedItem->SetLevel(atoi(level->Attribute("num")));

			// Create map key.
			ItemKey key;
			key.name = (ItemName)mStringKeyMap[name];
			key.level = loadedItem->GetLevel();

			// Add to map.
			mItemMap[key] = loadedItem;
		}
	}
}

void ItemLoaderXML::LoadStatusEffects(TiXmlElement* pRoot)
{
	TiXmlElement* statusEffects = pRoot->FirstChildElement("StatusEffects");
	for(TiXmlElement* effect = statusEffects->FirstChildElement("Effect"); effect != NULL; effect = effect->NextSiblingElement("Effect"))
	{
		string name = effect->Attribute("name") == NULL ? "#NOVALUE" : effect->Attribute("name");
		string desc = effect->FirstChildElement("Desc")->GetText();
		string icon = effect->Attribute("icon") == NULL ? "#NOVALUE" : effect->Attribute("icon");

		// Set the attributes and the other data.
		Item* loadedItem = new Item(icon);
		loadedItem->SetName(StringToName(name));
		loadedItem->SetDescription(desc);
		Attributes attributes;
		attributes.name = name;
		loadedItem->SetAttributes(attributes);

		// Create map key.
		ItemKey key;
		key.name = (ItemName)mStringKeyMap[name];
		key.level = loadedItem->GetLevel();

		// Add to map.
		mItemMap[key] = loadedItem;
	}
}

Item* ItemLoaderXML::GetItem(ItemKey key)
{
	if(mItemMap.find(key) != mItemMap.end())
		return mItemMap[key];
	else
		return nullptr;
}

ItemName ItemLoaderXML::StringToName(string name)
{
	return (ItemName)mStringKeyMap[name];
}