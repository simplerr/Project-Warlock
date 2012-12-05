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
	void PollAction(Client* pClient, GLib::Input* pInput);
	
	Skill* AddSkill(ItemName skillName);
	void RemoveSkill(ItemName name);
	void AddItem(BaseItem* pItem);
	void AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	void RemoveItem(BaseItem* pItem);
	void RemoveItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	multiset<ItemKey> GetItemList();
	std::map<int, Skill*> GetSkillMap();

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

	float GetHealth();
	float GetRegen();
	float GetKnockBackResistance();
	float GetLavaImmunity();
	float GetDamage();
	float GetLifeSteal();
	int	  GetGold();
	bool  GetEliminated();

	bool IsCastingSkill();
private:
	RakNet::SystemAddress mSystemAdress;
	SkillHandler*		  mSkillHandler;	// Should it really be here?
	multiset<ItemKey>	  mItemList;
	Attributes			  mAttributes;
	GLib::StaticObject*	  mLocalBox;	//[HACK]
	bool				  mEliminated;
	bool				  mLocalPlayer;
	int mGold;
};