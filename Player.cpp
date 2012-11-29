#include "Player.h"
#include "Input.h"
#include "World.h"
#include "Client.h"
#include "Graphics.h"
#include "Client.h"
#include "SkillHandler.h"

Player::Player()
	: Actor(GLib::GetGraphics()->GetModelImporter(), "models/smith/smith.x")
{
	SetType(GLib::PLAYER);
	SetHealth(100.0f);
	SetGold(10);
	mSkillHandler = new SkillHandler();
}

Player::~Player()
{
	delete mSkillHandler;
}

void Player::Init()
{

}

void Player::Update(float dt)
{
	Actor::Update(dt);
}

void Player::Draw(GLib::Graphics* pGraphics)
{
	Actor::Draw(pGraphics);
}

void Player::PollAction(Client* pClient, GLib::Input* pInput)
{
	// [TODO] Add mSkillHandler->PollAction().
	mSkillHandler->PollAction(pClient, pInput, GetPosition(), GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay()));

	if(pInput->KeyPressed('W'))
		SetVelocity(XMFLOAT3(0, 0, 0.1f));

	// Add movement target for the selected object.
	if(pInput->KeyPressed(VK_RBUTTON))
	{
		XMFLOAT3 pos = GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay());

		// Inform the server about what happened.
		// The server then informs all the clients, including the callee.
		if(pos.x != numeric_limits<float>::infinity())
			pClient->SendAddTarget(GetId(), pos, pInput->KeyDown(VK_SHIFT) ? false : true);
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

bool Player::IsCastingSkill()
{
	return mSkillHandler->IsCastingSkill();
}

void Player::AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey)
{
	mItemList.insert(itemKey);

	// Add item attributes.
	Item item = pItemLoader->GetItem(itemKey);
	SetHealth(GetHealth() + item.health);
	SetRegen(GetRegen() + item.regen);
	SetKnockBackResistance(GetKnockBackResistance() + item.knockbakResistance);
	SetLavaImmunity(GetLavaImmunity() + item.lavaImmunity);
	SetDamage(GetDamage() + item.damage);
	SetLifeSteal(GetLifeSteal() + item.lifesteal);
	SetMovementSpeed(GetMovementSpeed() + item.movementSpeed);
}

void Player::RemoveItem(ItemLoaderXML* pItemLoader, ItemKey itemKey)
{
	auto iter = mItemList.find(itemKey);
	if(iter != mItemList.end())
	{
		mItemList.erase(iter);

		// Remove item attributes.
		Item item = pItemLoader->GetItem(itemKey);
		SetHealth(GetHealth() - item.health);
		SetRegen(GetRegen() - item.regen);
		SetKnockBackResistance(GetKnockBackResistance() - item.knockbakResistance);
		SetLavaImmunity(GetLavaImmunity() - item.lavaImmunity);
		SetDamage(GetDamage() - item.damage);
		SetLifeSteal(GetLifeSteal() - item.lifesteal);
		SetMovementSpeed(GetMovementSpeed() - item.movementSpeed);
	}
}

multiset<ItemKey> Player::GetItemList()
{
	return mItemList;
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