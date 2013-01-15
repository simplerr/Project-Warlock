#pragma once
#include "Actor.h"
#include "RakPeerInterface.h"
#include "ItemLoaderXML.h"
#include "Skills.h"
#include "Items.h"
#include "StatusEffect.h"
#include <set>
#include <boost\function.hpp>
#include <boost\bind.hpp>

using namespace std;

namespace GLib {
	class Input;
	class StaticObject;
}

class SkillHandler;
class Client;
class ItemLoaderXML;
class Skill;
class StatusEffect;

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
	void RemoveStatusEffects();

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
	void SetStunned(bool stunned);

	float GetHealth();
	float GetRegen();
	float GetKnockBackResistance();
	float GetLavaImmunity();
	float GetDamage();
	float GetLifeSteal();
	int	  GetGold();
	bool  GetEliminated();
	bool  GetStunned();
	Player* GetLastHitter();
	AxisAlignedBox GetBoundingBox();
	list<StatusEffect*> GetStatusEffectList();
	multiset<ItemKey> GetItemList();

	// Status effect stuff.
	void AddStatusEffect(StatusEffect* pStatusEffect);

	template <class T>
	void AddOnStatusEffectAdded(void(T::*_callback)(StatusEffectType), T* _object)	{
		OnStatusEffectAdded = boost::bind(_callback, _object, _1);
	}

	template <class T>
	void AddOnStatusEffectRemoved(void(T::*_callback)(StatusEffectType), T* _object)	{
		OnStatusEffectRemoved = boost::bind(_callback, _object, _1);
	}

	boost::function<void(StatusEffectType)> OnStatusEffectAdded;
	boost::function<void(StatusEffectType)> OnStatusEffectRemoved;
private:
	RakNet::SystemAddress mSystemAdress;
	multiset<ItemKey>	  mItemList;
	Attributes			  mAttributes;
	GLib::StaticObject*	  mLocalBox;	//[HACK]
	Player*				  mLastHitter;
	list<StatusEffect*>	  mStatusEffects;
	bool				  mEliminated;
	bool				  mLocalPlayer;
	bool				  mStunned;
	int mGold;
};