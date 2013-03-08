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
#include "Camera.h"
#include "SkinnedModel.h"

#define NOT_DEAD 9999999999

Player::Player()
	: Actor(GLib::GetGraphics()->GetModelImporter(), "models/wizard/sorceress_anims_split.x")
{
	SetType(GLib::PLAYER);
	SetMaxHealth(100.0f);
	SetGold(100);
	SetLocalPlayer(false);
	SetEliminated(false);
	SetLastHitter(nullptr);
	SetStunned(false);
	SetMovementSpeed(0.025f);
	SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));	
	//GetModel()->SetMeshMaterial(2, GLib::Material(GLib::Colors::Red));

	mRedHealthBarTexture = GLib::GetGraphics()->LoadTexture("textures/bar.bmp");
	mGreenHealthBarTexture = GLib::GetGraphics()->LoadTexture("textures/green_bar.bmp");

	mDeathTimer = NOT_DEAD;
}

Player::~Player()
{
	
}

void Player::Init()
{
	SetCurrentHealth(GetMaxHealth());
	mDeathTimer = NOT_DEAD;
	SetEliminated(false);
}

void Player::Update(float dt)
{
	mDeathTimer -= dt;

	if(mDeathTimer <= 0)
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

		// Draw the health in 2D coordinates.
		XMFLOAT2 pos = pGraphics->TransformToScreenSpace(GetPosition() + XMFLOAT3(0, 6, 0));

		float maxWidth = 60;
		float barWidth = maxWidth * (GetCurrentHealth() / GetMaxHealth());
		barWidth = max(barWidth, 0);
		float barPosX = pos.x - maxWidth/2 + barWidth/2;

		if(!mLocalPlayer)
			pGraphics->DrawScreenQuad(mRedHealthBarTexture, barPosX, pos.y-20, barWidth, 10);
		else
			pGraphics->DrawScreenQuad(mGreenHealthBarTexture, barPosX, pos.y-20, barWidth, 10);

		float nameWidth = pGraphics->MeasureText(GetName(), 20, "Arial").Width();
		pGraphics->DrawText(GetName(), pos.x - nameWidth/2, pos.y-50, 20);
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
	SetMaxHealth(GetCurrentHealth() + attributes.health);
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
		SetMaxHealth(GetCurrentHealth() - attributes.health);
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

void Player::SetCurrentHealth(float health)
{
	mCurrentHealth = health;
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

float Player::GetCurrentHealth()
{
	return mCurrentHealth;
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

float Player::GetMovementSpeed()
{
	return mAttributes.movementSpeed;
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

float Player::GetMaxHealth()
{
	return mAttributes.health;
}

void Player::TakeDamage(float damage)
{
	mCurrentHealth -= damage;
}

void Player::SetMaxHealth(float maxHealth)
{
	mAttributes.health = maxHealth;
	mCurrentHealth = maxHealth;
}

void Player::SetMovementSpeed(float movementSpeed)
{
	mAttributes.movementSpeed = movementSpeed;
}

void Player::SetDeathAnimation()
{
	SetAnimation(7, 1.0f);
	mDeathTimer = 1.0f;
}

float Player::GetDeathTimer()
{
	return mDeathTimer;
}

void Player::SetDeathTimer(float timer)
{
	mDeathTimer = timer;
}