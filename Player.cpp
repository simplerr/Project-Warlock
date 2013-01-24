#include "Player.h"
#include "Input.h"
#include "World.h"
#include "Client.h"
#include "Graphics.h"
#include "Client.h"
#include "SkillHandler.h"
#include "StaticObject.h"
#include "ModelImporter.h"
#include "StatusEffect.h"

Player::Player()
	: Actor(GLib::GetGraphics()->GetModelImporter(), "models/smith/smith.x")
{
	SetType(GLib::PLAYER);
	SetHealth(100.0f);
	SetGold(100);
	SetLocalPlayer(false);
	SetEliminated(false);
	SetLastHitter(nullptr);
	SetStunned(false);

	mLocalBox = new GLib::StaticObject(GLib::GetGraphics()->GetModelImporter(), "models/box.obj");
	mLocalBox->SetMaterials(GLib::Colors::Green);
	mLocalBox->SetScale(XMFLOAT3(0.6, 0.6, 0.6));
}

Player::~Player()
{
	
}

void Player::Init()
{
	SetMovementSpeed(0.025f);
	SetHealth(100.0f);
}

void Player::Update(float dt)
{
	if(GetHealth() <= 0) 
		SetEliminated(true);

	if(!GetEliminated())
	{
		if(!GetStunned())
			Actor::Update(dt);

		for(auto iter = mStatusEffects.begin(); iter != mStatusEffects.end(); iter++)
			(*iter)->Update(dt);
	}
}

void Player::Draw(GLib::Graphics* pGraphics)
{
	if(!GetEliminated())
	{
		Actor::Draw(pGraphics);

		mLocalBox->SetPosition(GetPosition() + XMFLOAT3(0, 6, 0));

		if(mLocalPlayer)
			mLocalBox->Draw(pGraphics);

		for(auto iter = mStatusEffects.begin(); iter != mStatusEffects.end(); iter++) {
			(*iter)->Draw(pGraphics);
			if((*iter)->GetTimer() > (*iter)->GetDuration()) {
				// Call the OnStatusEffectRemoved callback.
				if(!OnStatusEffectRemoved.empty())
					OnStatusEffectRemoved((*iter)->GetType());

				(*iter)->Remove();
				//delete (*iter);	// [NOTE][HACK] Memory leak!!
				iter = mStatusEffects.erase(iter);
			}
		}
	}
}

void Player::SetSystemAdress(RakNet::SystemAddress adress)
{
	mSystemAdress = adress;
}

RakNet::SystemAddress Player::GetSystemAdress()
{
	return mSystemAdress;
}

void Player::AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey)
{
	AddItem(pItemLoader->GetItem(itemKey));
}

void Player::AddItem(HudItem* pItem)
{
	mItemList.insert(ItemKey(pItem->GetName(), pItem->GetLevel()));

	Attributes attributes = pItem->GetAttributes();
	SetHealth(GetHealth() + attributes.health);
	SetRegen(GetRegen() + attributes.regen);
	SetKnockBackResistance(GetKnockBackResistance() + attributes.knockbackResistance);
	SetLavaImmunity(GetLavaImmunity() + attributes.lavaImmunity);
	SetDamage(GetDamage() + attributes.damage);
	SetLifeSteal(GetLifeSteal() + attributes.lifesteal);
	SetMovementSpeed(GetMovementSpeed() + attributes.movementSpeed);
}

void Player::RemoveItem(HudItem* pItem)
{
	ItemKey key = ItemKey(pItem->GetName(), pItem->GetLevel());
	auto iter = mItemList.find(key);
	if(iter != mItemList.end())
	{
		// Remove from item set.
		mItemList.erase(iter);

		// Remove item attributes.
		Attributes attributes = pItem->GetAttributes();
		SetHealth(GetHealth() - attributes.health);
		SetRegen(GetRegen() - attributes.regen);
		SetKnockBackResistance(GetKnockBackResistance() - attributes.knockbackResistance);
		SetLavaImmunity(GetLavaImmunity() - attributes.lavaImmunity);
		SetDamage(GetDamage() - attributes.damage);
		SetLifeSteal(GetLifeSteal() - attributes.lifesteal);
		SetMovementSpeed(GetMovementSpeed() - attributes.movementSpeed);
	}
}

void Player::RemoveItem(ItemLoaderXML* pItemLoader, ItemKey itemKey)
{
	RemoveItem(pItemLoader->GetItem(itemKey));
}

multiset<ItemKey> Player::GetItemList()
{
	return mItemList;
}

void Player::AddStatusEffect(StatusEffect* pStatusEffect)
{
	// Status effects can't stack!
	for(auto iter = mStatusEffects.begin(); iter != mStatusEffects.end(); iter++) {
		if((*iter)->GetType() == pStatusEffect->GetType())
			return;
	}

	if(!OnStatusEffectAdded.empty())
		OnStatusEffectAdded(pStatusEffect->GetType());

	mStatusEffects.push_back(pStatusEffect);
	pStatusEffect->SetPlayer(this);
	pStatusEffect->Apply();
}

void Player::SetHealth(float health)
{
	mAttributes.health = health;
}

void Player::SetRegen(float regen)
{
	mAttributes.regen = regen;
}

void Player::SetKnockBackResistance(float resistance)
{
	mAttributes.knockbackResistance = resistance;
}

void Player::SetLavaImmunity(float immunity)
{
	mAttributes.lavaImmunity = immunity;
}

void Player::SetDamage(float damage)
{
	mAttributes.damage = damage;
}

void Player::SetLifeSteal(float lifesteal)
{
	mAttributes.lifesteal = lifesteal;
}

void Player::SetGold(int gold)
{
	mGold = gold;
}

void Player::SetLocalPlayer(bool local)
{
	mLocalPlayer = local;
}

float Player::GetHealth()
{
	return mAttributes.health;
}

float Player::GetRegen()
{
	return mAttributes.regen;
}

float Player::GetKnockBackResistance()
{
	return mAttributes.knockbackResistance;
}

float Player::GetLavaImmunity()
{
	return mAttributes.lavaImmunity;
}

float Player::GetDamage()
{
	return mAttributes.damage;
}

float Player::GetLifeSteal()
{
	return mAttributes.lifesteal;
}

int	Player::GetGold()
{
	return mGold;
}

void Player::SetEliminated(bool eliminated)
{
	mEliminated = eliminated;
}

bool Player::GetEliminated()
{
	return mEliminated;
}

void Player::SetLastHitter(Player* pPlayer)
{
	mLastHitter = pPlayer;
}

Player* Player::GetLastHitter()
{
	return mLastHitter;
}

AxisAlignedBox Player::GetBoundingBox()
{
	XNA::AxisAlignedBox box;
	box.Center = GetPosition();
	box.Extents = XMFLOAT3(2, 2, 2);
	return box;
}

void Player::SetStunned(bool stunned)
{
	mStunned = stunned;

	if(mStunned)
		ClearTargetQueue();
}

bool Player::GetStunned()
{
	return mStunned;
}

list<StatusEffect*> Player::GetStatusEffectList()
{
	return mStatusEffects;
}

void Player::RemoveStatusEffects()
{
	for(auto iter = mStatusEffects.begin(); iter != mStatusEffects.end(); iter++) {
		// Call the OnStatusEffectRemoved callback.
		if(!OnStatusEffectRemoved.empty())
			OnStatusEffectRemoved((*iter)->GetType());

		(*iter)->Remove();
		//delete (*iter); // [NOTE][HACK] Memory leak!!
		iter = mStatusEffects.erase(iter);
	}
}