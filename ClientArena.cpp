#include "ClientArena.h"
#include "BloodParticles.h"
#include "Client.h"
#include "World.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Object3D.h"
#include "NetworkMessages.h"
#include "Player.h"
#include "UserInterface.h"
#include "Input.h"
#include "ParticleSystem.h"
#include "PlayerModule.h"
#include "StaticObject.h"

ClientArena::ClientArena(Client* pClient)
	: BaseArena()
{
	mClient = pClient;

	mSelectedPlayer = nullptr;
	mPlayer			= new PlayerModule();

	mWorld->AddObjectAddedListener(&ClientArena::OnObjectAdded, this);
	mWorld->AddObjectRemovedListener(&ClientArena::OnObjectRemoved, this);

	for(int i = 0; i < 40; i++)
	{
		string plant;
		int plantId = rand() % 3 + 1;
		if(plantId == 1)
			plant = "models/misc/PlantReed.obj";
		else if(plantId == 2)
			plant = "models/misc/PlantFlowers.obj";
		else if(plantId == 3)
			plant = "models/misc/PlantGrass.obj";

		GLib::StaticObject* object = new GLib::StaticObject(GLib::GetGraphics()->GetModelImporter(), plant);
		object->SetPosition(XMFLOAT3(40 - rand() % 80, 1, 40 - rand() % 80));
		mWorld->AddObject(object);
		object->SetId(i +1000);
	}
}

ClientArena::~ClientArena()
{
	delete mPlayer;
}

void ClientArena::Update(GLib::Input* pInput, float dt)
{
	// Update the world.
	mWorld->Update(dt);
	mPlayer->Update(dt);

	// Poll for object selection.
	PollSelection(pInput);

	// If the selected object is the player then poll for action.
	if(mSelectedPlayer != nullptr && mSelectedPlayer->GetId() == mPlayer->GetPlayer()->GetId() && !mClient->GetUi()->PointInsideUi(pInput->MousePosition())) 
		mPlayer->PollAction(mClient, pInput);
}

void ClientArena::Draw(GLib::Graphics* pGraphics)
{
	// Draw the world.
	mWorld->Draw(pGraphics);

	/*if(mSelectedPlayer != nullptr) {
		char buffer[244];
		sprintf(buffer, "Health: %.2f\nRegen: %.2f\nMs: %.2f\nKnockbak res: %.2f\nLava Immunity: %.2f\nDamage: %.2f\nLifesteal: %.2f\n\nGold: %i\n\n", mSelectedPlayer->GetHealth(), mSelectedPlayer->GetRegen(), mSelectedPlayer->GetMovementSpeed(),
			mSelectedPlayer->GetKnockBackResistance(), mSelectedPlayer->GetLavaImmunity(), mSelectedPlayer->GetDamage(), mSelectedPlayer->GetLifeSteal(), mSelectedPlayer->GetGold());
		pGraphics->DrawText(buffer, 10, 100, 16, 0xff000000);
	}*/
}

void ClientArena::PollSelection(GLib::Input* pInput)
{
	// Get the selected object.
	if(pInput->KeyPressed(VK_LBUTTON) && !mPlayer->IsCastingSkill() && !mClient->GetUi()->PointInsideUi(pInput->MousePosition()))
	{
		Player* selected = (Player*)mWorld->GetSelectedObject(pInput->GetWorldPickingRay(), GLib::PLAYER);

		if(selected != nullptr)
			mClient->GetUi()->SetSelectedPlayer(GetPlayerModule(selected->GetId()));
	}
}

void ClientArena::SetSelectedPlayer(Player* pPlayer)
{
	if(pPlayer != nullptr) 
	{
		if(mSelectedPlayer != nullptr) {
			mSelectedPlayer->SetSelected(false);
			mSelectedPlayer->SetMaterials(GLib::Material(GLib::Colors::White));
		}

		mSelectedPlayer = pPlayer;
		mSelectedPlayer->SetSelected(true);
		mSelectedPlayer->SetMaterials(GLib::Material(GLib::Colors::White));
		//mSelectedPlayer->SetMaterials(GLib::Material(XMFLOAT4(1.0f, 127.0f/255.0f, 38/255.0f, 0.12f) * 4));
		mClient->GetUi()->SetSelectedPlayer(GetPlayerModule(pPlayer->GetId()));
	}
}

//! Removes a player from mPlayerList.
void ClientArena::RemovePlayer(int id)
{
	// Loop through all objects and find out which one to delete.
	for(auto iter =  mPlayerList.begin(); iter != mPlayerList.end(); iter++)
	{
		if((*iter)->GetId() == id) {
			mPlayerList.erase(iter);
			break;
		}
		else	
			iter++;
	}
}

//! Gets called in World::AddObject().
void ClientArena::OnObjectAdded(GLib::Object3D* pObject)
{
	// Add player to mPlayerList.
	if(pObject->GetType() == GLib::PLAYER) {
		mPlayerList.push_back((Player*)pObject);
	
		PlayerModule* module = new PlayerModule();
		module->SetPlayer((Player*)pObject);
		mModuleList[pObject->GetId()] = module;

		if(mClient->GetName() == pObject->GetName() && mClient->GetLocalPlayer() == nullptr)
			SetLocalModule(module);
	}
}

//! Gets called in World::RemoveObject().
void ClientArena::OnObjectRemoved(GLib::Object3D* pObject)
{
	// Remove player from mPlayerList:
	if(pObject->GetType() == GLib::PLAYER) 
		RemovePlayer(pObject->GetId());

	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_OBJECT_REMOVED);
	bitstream.Write(pObject->GetId());
	mClient->SendServerMessage(bitstream);
}

void ClientArena::ResetPlayers()
{
	// [NOTE] The position is set by the server. Then passed by NMSG_UPDATEW_WORLD.
	for(int i = 0; i < mPlayerList.size(); i++) 
	{
		mPlayerList[i]->Init();
		mPlayerList[i]->SetEliminated(false);
		mPlayerList[i]->SetCurrentHealth(mPlayerList[i]->GetMaxHealth());	// [NOTE][HACK]
		mPlayerList[i]->RemoveStatusEffects();
		mPlayerList[i]->ClearTargetQueue();
	}
}

GLib::World* ClientArena::GetWorld()
{
	return mWorld;
}

Player*	ClientArena::GetLocalPlayer()
{
	return mPlayer->GetPlayer();
}

void ClientArena::SetLocalModule(PlayerModule* pModule)
{
	pModule->GetPlayer()->SetLocalPlayer(true);
	mPlayer = pModule;

	// Setup the StatusEffect callbacks.
	Player* player = pModule->GetPlayer();
	player->AddOnStatusEffectAdded(&UserInterface::OnStatusEffectAdded, mClient->GetUi());
	player->AddOnStatusEffectRemoved(&UserInterface::OnStatusEffectRemoved, mClient->GetUi());
}

bool ClientArena::IsLocalPlayerSelected()
{
	return (mSelectedPlayer != nullptr && mSelectedPlayer->GetId() == mPlayer->GetPlayer()->GetId());
}

vector<Player*> ClientArena::GetPlayerList()
{
	return mPlayerList;
}

PlayerModule* ClientArena::GetLocalPlayerModule()
{
	return mPlayer;
}

PlayerModule* ClientArena::GetPlayerModule(int id)
{
	if(mModuleList.find(id) != mModuleList.end())
		return mModuleList[id];
	else
		return nullptr;
}