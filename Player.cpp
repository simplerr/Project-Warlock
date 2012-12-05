#include "Player.h"
#include "Input.h"
#include "World.h"
#include "Client.h"
#include "Graphics.h"
#include "Client.h"
#include "SkillHandler.h"
#include "StaticObject.h"
#include "ModelImporter.h"

Player::Player()
	: Actor(GLib::GetGraphics()->GetModelImporter(), "models/smith/smith.x")
{
	SetType(GLib::PLAYER);
	SetHealth(100.0f);
	SetGold(100);
	SetLocalPlayer(false);
	SetEliminated(false);
	mSkillHandler = new SkillHandler();

	mLocalBox = new GLib::StaticObject(GLib::GetGraphics()->GetModelImporter(), "models/box.obj");
	mLocalBox->SetMaterials(GLib::Colors::Green);
	mLocalBox->SetScale(XMFLOAT3(0.6, 0.6, 0.6));
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
	if(GetHealth() <= 0)
		SetEliminated(true);

	if(!GetEliminated())
		Actor::Update(dt);

	mSkillHandler->Update(dt);
}

void Player::Draw(GLib::Graphics* pGraphics)
{
	if(!GetEliminated())
	{
		Actor::Draw(pGraphics);

		mLocalBox->SetPosition(GetPosition() + XMFLOAT3(0, 6, 0));

		if(mLocalPlayer)
			mLocalBox->Draw(pGraphics);
	}
}

void Player::PollAction(Client* pClient, GLib::Input* pInput)
{
	if(!GetEliminated())
	{
		// [TODO] Add mSkillHandler->PollAction().
		mSkillHandler->PollAction(pClient, pInput, GetPosition(), GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay()));

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

Skill* Player::AddSkill(ItemName skillName)
{
	return mSkillHandler->AddSkill(skillName);
}

void Player::RemoveSkill(ItemName name)
{
	mSkillHandler->RemoveSkill(name);
}

void Player::AddItem(ItemLoaderXML* pItemLoader, ItemKey itemKey)
{
	AddItem(pItemLoader->GetItem(itemKey));
}

void Player::AddItem(BaseItem* pItem)
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

void Player::RemoveItem(BaseItem* pItem)
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

std::map<int, Skill*> Player::GetSkillMap()
{
	return mSkillHandler->GetSkillMap();
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