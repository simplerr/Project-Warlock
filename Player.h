#pragma once
#include "Actor.h"
#include "RakPeerInterface.h"
#include "ItemLoaderXML.h"
#include <set>

using namespace std;

namespace GLib {
	class Input;
}

class SkillHandler;
class Client;
class ItemLoaderXML;

struct PlayerAttributes
{
	PlayerAttributes() {
		regen = knockbackResistance = lavaImmunity = damage = lifesteal = 0.0f;
		health = 100.0f;
	}

	float health;
	float regen;
	float knockbackResistance;
	float lavaImmunity;
	float damage;
	float lifesteal;
};

class Player : public Actor
{
public:
	Player();
	~Player();

	void Init();
	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);
	void PollAction(Client* pClient, GLib::Input* pInput);
	
	void AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	void RemoveItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	multiset<ItemKey> GetItemList();

	void SetSystemAdress(RakNet::SystemAddress adress);
	RakNet::SystemAddress GetSystemAdress();

	void SetHealth(float health);
	void SetRegen(float regen);
	void SetKnockBackResistance(float resistance);
	void SetLavaImmunity(float immunity);
	void SetDamage(float damage);
	void SetLifeSteal(float lifesteal);

	float GetHealth();
	float GetRegen();
	float GetKnockBackResistance();
	float GetLavaImmunity();
	float GetDamage();
	float GetLifeSteal();

	bool IsCastingSkill();
private:
	RakNet::SystemAddress mSystemAdress;
	SkillHandler*		  mSkillHandler;	// Should it really be here?
	multiset<ItemKey>		  mItemList;
	PlayerAttributes	  mAttributes;
};