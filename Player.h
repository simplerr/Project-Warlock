#pragma once
#include "Actor.h"
#include "RakPeerInterface.h"
#include "ItemLoaderXML.h"
#include "Skills.h"
#include "Items.h"
#include <set>

using namespace std;

namespace GLib {
	class Input;
	class StaticObject;
}

class SkillHandler;
class Client;
class ItemLoaderXML;
class Skill;

class Player : public Actor
{
public:
	Player();
	~Player();

	void Init();
	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);
	
	void AddItem(BaseItem* pItem);
	void AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	void RemoveItem(BaseItem* pItem);
	void RemoveItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	multiset<ItemKey> GetItemList();

	void SetSystemAdress(RakNet::SystemAddress adress);
	RakNet::SystemAddress GetSystemAdress();

	void SetLocalPlayer(bool local);
	void SetHealth(float health);
	void SetRegen(float regen);
	void SetKnockBackResistance(float resistance);
	void SetLavaImmunity(float immunity);
	void SetDamage(float damage);
	void SetLifeSteal(float lifesteal);
	void SetGold(int gold);
	void SetEliminated(bool eliminated);
	void SetLastHitter(Player* pPlayer);

	float GetHealth();
	float GetRegen();
	float GetKnockBackResistance();
	float GetLavaImmunity();
	float GetDamage();
	float GetLifeSteal();
	int	  GetGold();
	bool  GetEliminated();
	Player* GetLastHitter();
private:
	RakNet::SystemAddress mSystemAdress;
	multiset<ItemKey>	  mItemList;
	Attributes			  mAttributes;
	GLib::StaticObject*	  mLocalBox;	//[HACK]
	Player*				  mLastHitter;
	bool				  mEliminated;
	bool				  mLocalPlayer;
	int mGold;
};