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
	
	void AddItem(HudItem* pItem);
	void AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	void RemoveItem(HudItem* pItem);
	void RemoveItem(ItemLoaderXML* pItemLoader, ItemKey itemKey);
	void RemoveStatusEffects();

	void SetSystemAdress(RakNet::SystemAddress adress);
	RakNet::SystemAddress GetSystemAdress();

	void SetLocalPlayer(bool local);
	void SetCurrentHealth(float health);
	void SetMaxHealth(float maxHealth);
	void TakeDamage(float damage);
	void SetRegen(float regen);
	void SetKnockBackResistance(float resistance);
	void SetLavaImmunity(float immunity);
	void SetDamage(float damage);
	void SetLifeSteal(float lifesteal);
	void SetGold(int gold);
	void SetEliminated(bool eliminated);
	void SetLastHitter(Player* pPlayer);
	void SetStunned(bool stunned);
	void SetMovementSpeed(float movementSpeed);
	void SetDeathAnimation();
	void SetDeathTimer(float timer);

	float GetCurrentHealth();
	float GetMaxHealth();
	float GetRegen();
	float GetKnockBackResistance();
	float GetLavaImmunity();
	float GetBonusDamage();
	float GetLifeSteal();
	float GetMovementSpeed();
	float GetDeathTimer();
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
	void AddOnStatusEffectAdded(void(T::*_callback)(ItemName), T* _object)	{
		OnStatusEffectAdded = boost::bind(_callback, _object, _1);
	}

	template <class T>
	void AddOnStatusEffectRemoved(void(T::*_callback)(ItemName), T* _object)	{
		OnStatusEffectRemoved = boost::bind(_callback, _object, _1);
	}

	boost::function<void(ItemName)> OnStatusEffectAdded;
	boost::function<void(ItemName)> OnStatusEffectRemoved;
private:
	RakNet::SystemAddress mSystemAdress;
	multiset<ItemKey>	  mItemList;
	Attributes			  mAttributes;
	Player*				  mLastHitter;
	list<StatusEffect*>	  mStatusEffects;
	GLib::Texture2D*	  mRedHealthBarTexture;
	GLib::Texture2D*	  mGreenHealthBarTexture;
	bool				  mEliminated;
	bool				  mLocalPlayer;
	bool				  mStunned;
	float				  mCurrentHealth;
	int					  mGold;
	float				  mDeathTimer;
};