#include "ClientMessageHandler.h"
#include "UserInterface.h"
#include "ClientArena.h"
#include "Sound.h"
#include "Client.h"
#include "World.h"
#include "Object3D.h"
#include "Player.h"
#include "NetworkMessages.h"
#include "ClientSkillInterpreter.h"
#include "RoundHandler.h"
#include "Projectile.h"
#include "FreezeEffect.h"
#include "ServerCvars.h"

ClientMessageHandler::ClientMessageHandler(Client* pClient)
{
	mClient = pClient;
	mSkillInterpreter = new ClientSkillInterpreter();
}

ClientMessageHandler::~ClientMessageHandler()
{
	delete mSkillInterpreter;
}

void ClientMessageHandler::HandleWorldUpdate(RakNet::BitStream& bitstream)
{
	GLib::ObjectType type;
	int id;
	XMFLOAT3 pos, rotation;
	float health, deathTimer;
	int animIndex, gold, eliminated;

	bitstream.Read(type);
	bitstream.Read(id);
	bitstream.Read(pos.x);
	bitstream.Read(pos.y);
	bitstream.Read(pos.z);
	bitstream.Read(rotation.x);
	bitstream.Read(rotation.y);
	bitstream.Read(rotation.z);

	if(type == GLib::PLAYER)
	{
		bitstream.Read(animIndex);
		bitstream.Read(deathTimer);
		bitstream.Read(health);
		bitstream.Read(gold);
		bitstream.Read(eliminated);
	}

	GLib::ObjectList* objects = mClient->GetWorld()->GetObjects();
	for(auto iter = objects->begin(); iter != objects->end(); iter++)
	{
		GLib::Object3D* object = (*iter);

		// Do interpolation stuff here.
		if(object->GetId() == id)
		{
			object->SetPosition(pos);
			object->SetRotation(rotation);

			if(object->GetType() == GLib::PLAYER) 
			{
				Player* player = (Player*)object;
				player->SetAnimation(animIndex);
				player->SetDeathTimer(deathTimer);
				player->SetCurrentHealth(health);
				player->SetGold(gold);
				player->SetEliminated(eliminated);
			}
		}
	}
}

void ClientMessageHandler::HandleTargetAdded(RakNet::BitStream& bitstream)
{
	char name[244];
	unsigned char id;
	float x, y, z;
	bool clear;
	bitstream.Read(name);

	/*if(string(name) == mName)
		break;*/
	bitstream.Read(id);
	bitstream.Read(x);
	bitstream.Read(y);
	bitstream.Read(z);
	bitstream.Read(clear);

	auto playerList = mClient->GetPlayerList();
	for(int i = 0; i < playerList.size(); i++)
	{
		Actor* object = (Actor*)playerList[i];
		if(object->GetId() == id)
			object->AddTarget(XMFLOAT3(x, y, z), clear);
	}
}

void ClientMessageHandler::HandleObjectRemvoed(RakNet::BitStream& bitstream)
{
	int id;
	bitstream.Read(id);

	GLib::Object3D* object = mClient->GetWorld()->GetObjectById(id);
	if(object != nullptr)
		object->Kill();
	//mClient->GetWorld()->RemoveObject(id);
}

void ClientMessageHandler::HandleConnectionSuccess(RakNet::BitStream& bitstream)
{
	// Add the players already connected.
	char name[244];
	int numPlayers, id;
	XMFLOAT3 pos;
	CurrentState state;

	bitstream.Read(state);
	bitstream.Read(numPlayers);

	mClient->GetRoundHandler()->SetCurrentState(state);

	for(int i = 0; i < numPlayers; i++)
	{
		bitstream.Read(name);
		bitstream.Read(id);
		bitstream.Read(pos);

		// Add a new player to the World.
		Player* player = new Player();
		player->SetName(name);
		player->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));	
		player->SetPosition(pos);
		mClient->GetWorld()->AddObject(player, id); // Use the servers ID.
		mClient->GetRoundHandler()->AddScore(name, 0);
	}

	// Send the client info to the server (name etc).
	RakNet::BitStream sendBitstream;
	sendBitstream.Write((unsigned char)NMSG_CLIENT_CONNECTION_DATA);
	sendBitstream.Write(mClient->GetName().c_str());
	mClient->SendServerMessage(sendBitstream);
}

void ClientMessageHandler::HandleAddPlayer(RakNet::BitStream& bitstream)
{
	char buffer[244];
	int id, gold;
	bitstream.Read(buffer);
	bitstream.Read(id);
	bitstream.Read(gold);

	string name = buffer;

	// Add a new player to the World.
	Player* player = new Player();
	player->SetName(name);
	mClient->GetWorld()->AddObject(player, id);		// Callback that calls ClientArena::OnObjectAdded(), this adds Players to the ModuleMap.
	player->SetGold(gold);

	// Add to score map.
	mClient->GetRoundHandler()->AddScore(name, 0);

	if(mClient->GetName() == name) {// && mClient->GetLocalPlayer() == nullptr) {
		mClient->AddChatText("Successfully connected to the server!\n", RGB(0, 200, 0));
		mClient->SetSelectedPlayer(player);
	}
	else
		mClient->AddChatText(name + " has connected!\n", RGB(0, 200, 0));

	gSound->PlayEffect("data/sounds/player_connect.wav");
}

void ClientMessageHandler::HandlePlayerDisconnected(RakNet::BitStream& bitstream)
{
	char name[244];
	bitstream.Read(name);

	mClient->AddChatText(string(name) + " has disconnected!\n", RGB(200, 0, 0));
}

void ClientMessageHandler::HandleGetConnectedPlayers(RakNet::BitStream& bitstream)
{
	int numConnected;
	vector<string> names;

	bitstream.Read(numConnected);
	for(int i = 0; i < numConnected; i++)
	{
		char buffer[244];
		bitstream.Read(buffer);
		names.push_back(buffer);
	}

	mClient->AddChatText(string("Users connected:\n").c_str(), RGB(0, 0, 200));
	for(int i = 0; i < numConnected; i++)
		mClient->AddChatText(string(names[i] + "\n").c_str());
}

void ClientMessageHandler::HandleSkillCasted(RakNet::BitStream& bitstream)
{
	unsigned char skillCasted;
	bitstream.Read(skillCasted);
	mSkillInterpreter->Interpret(mClient, (MessageId)skillCasted, bitstream);
}

void ClientMessageHandler::HandleProjectilePlayerCollision(RakNet::BitStream& bitstream)
{
	int projectileId, playerId;
	bitstream.Read(projectileId);
	bitstream.Read(playerId);

	Player* player = (Player*)mClient->GetWorld()->GetObjectById(playerId);
	Projectile* projectile = (Projectile*)mClient->GetWorld()->GetObjectById(projectileId);
	projectile->HandlePlayerCollision(player, mClient->GetArena(), mClient->GetUi()->GetItemLoader());

	//MessageBox(0, "test!", 0, 0);

	// Play impact sound.
	gSound->PlayEffect(projectile->GetImpactSound());

	// Add status effect if there is any.
	StatusEffect* statusEffect = projectile->GetStatusEffect(mClient->GetUi()->GetItemLoader());
	if(statusEffect != nullptr)
		player->AddStatusEffect(statusEffect);
}

void ClientMessageHandler::HandleRoundStarted(RakNet::BitStream& bitstream)
{
	mClient->StartRound();
}

void ClientMessageHandler::HandleRoundEnded(RakNet::BitStream& bitstream)
{
	char winner[244];
	bitstream.Read(winner);
	mClient->EndRound(winner);
}

void ClientMessageHandler::HandleCvarList(RakNet::BitStream& bitstream)
{
	int startGold, shopTime, roundTime, numRounds, goldPerKill, goldPerWin, goldPerRound, lavaDamage, projectileImpulse, arenaRadius, floodInterval, floodSize, cheats;
	bitstream.Read(startGold);
	bitstream.Read(shopTime);
	bitstream.Read(roundTime);
	bitstream.Read(numRounds);
	bitstream.Read(goldPerKill);
	bitstream.Read(goldPerWin);
	bitstream.Read(goldPerRound);
	bitstream.Read(lavaDamage);
	bitstream.Read(projectileImpulse);
	bitstream.Read(arenaRadius);
	bitstream.Read(floodInterval);
	bitstream.Read(floodSize);
	bitstream.Read(cheats);

	// Format and add it to the chat.
	char buffer[2048];
	sprintf(buffer, "-startgold: %i\n-shoptime: %i\n-roundtime: %i\n-rounds: %i\n-goldperkill: %i\n-goldperwin: %i\n-goldperround: %i\n-lavadmg: %f\n-impulse: %f\n-radius: %f\n-flood_interval: %f\n-flood_size: %f\n-cheats: %f\n",
		startGold, shopTime, roundTime, numRounds, goldPerKill, goldPerWin, goldPerRound, lavaDamage, lavaDamage, arenaRadius, floodInterval, floodSize, cheats);

	mClient->AddChatText("Cvar list:\n", RGB(0, 0, 255));
	mClient->AddChatText(buffer, RGB(0, 0, 0));
}

void ClientMessageHandler::HandleCvarChange(RakNet::BitStream& bitstream)
{
	char temp[128];
	int value, show;

	bitstream.Read(temp);
	bitstream.Read(value);
	bitstream.Read(show);

	// Change cvar value.
	gCvars->SetCvarValue(temp, value);

	// Show in chat?
	if(show == 1)
	{
		string cvar = string(temp).substr(1, string(temp).size()-1);

		char buffer[244];
		sprintf(buffer, "%s set to %i\n", cvar.c_str(), value);

		// Add to chat.
		mClient->AddChatText(buffer, RGB(0, 200, 0));
	}
}

void ClientMessageHandler::HandlePlayerEliminated(RakNet::BitStream& bitstream)
{
	char killed[128];
	char eliminator[128];

	bitstream.Read(killed);
	bitstream.Read(eliminator);

	string chatText = string(killed) + " was pwnd by " + string(eliminator) + "!\n";
	mClient->AddChatText((char*)chatText.c_str(), RGB(255, 0, 0));
}

void ClientMessageHandler::HandlePerformRematch(RakNet::BitStream& bitstream)
{
	// Remove all the players items.
	mClient->GetUi()->RemoveAllItems();
}

void ClientMessageHandler::HandleServerShutdown(RakNet::BitStream& bitstream)
{
	mClient->GetUi()->ShowInGameMenu(true);
	mClient->GetUi()->AddStatusText("Server shutdown!", GLib::GetClientWidth()/2, 100, 10000, 75, GLib::ColorRGBA(255, 0, 0, 255));
}