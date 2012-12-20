#include "ClientMessageHandler.h"
#include "Client.h"
#include "World.h"
#include "Object3D.h"
#include "Player.h"
#include "NetworkMessages.h"
#include "ClientSkillInterpreter.h"

ClientMessageHandler::ClientMessageHandler(Client* pClient)
{
	mClient = pClient;
}

ClientMessageHandler::~ClientMessageHandler()
{

}

void ClientMessageHandler::HandleWorldUpdate(RakNet::BitStream& bitstream)
{
	GLib::ObjectType type;
	unsigned char id;
	XMFLOAT3 pos;
	float health;
	int gold;

	bitstream.Read(type);
	bitstream.Read(id);
	bitstream.Read(pos.x);
	bitstream.Read(pos.y);
	bitstream.Read(pos.z);

	if(type == GLib::PLAYER)
	{
		bitstream.Read(health);
		bitstream.Read(gold);
	}

	GLib::ObjectList* objects = mClient->GetWorld()->GetObjects();
	for(int i = 0; i < objects->size(); i++)
	{
		GLib::Object3D* object = objects->operator[](i);

		// Do interpolation stuff here.
		if(object->GetId() == id)
		{
			object->SetPosition(pos);

			if(object->GetType() == GLib::PLAYER) 
			{
				Player* player = (Player*)object;
				player->SetHealth(health);
				player->SetGold(gold);
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

	GLib::ObjectList* objects = mClient->GetWorld()->GetObjects();
	for(int i = 0; i < objects->size(); i++)
	{
		Actor* object = (Actor*)objects->operator[](i);
		if(object->GetId() == id)
			object->AddTarget(XMFLOAT3(x, y, z), clear);
	}
}

void ClientMessageHandler::HandleObjectRemvoed(RakNet::BitStream& bitstream)
{
	int id;
	bitstream.Read(id);
	mClient->GetWorld()->RemoveObject(id);
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

	mClient->SetArenaState(state);

	for(int i = 0; i < numPlayers; i++)
	{
		bitstream.Read(name);
		bitstream.Read(id);
		bitstream.Read(pos);

		// Add a new player to the World.
		Player* player = new Player();
		player->SetName(name);
		player->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));	
		player->SetPosition(pos);
		mClient->GetWorld()->AddObject(player);
		player->SetId(id);	// Use the servers ID.

		mClient->SetScore(name, 0);
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

	mClient->SetScore(name, 0);

	// Add a new player to the World.
	Player* player = new Player();
	player->SetName(name);
	player->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));	// [NOTE]
	mClient->GetWorld()->AddObject(player, id);		// Callback that calls ClientArena::OnObjectAdded(), this adds Players to the ModuleMap.
	player->SetGold(gold);

	if(mClient->GetName() == name && mClient->GetLocalPlayer() == nullptr) 
		mClient->AddChatText("Successfully connected to the server!\n", RGB(0, 200, 0));
	else
		mClient->AddChatText(name + " has connected!\n", RGB(0, 200, 0));
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
	mClient->GetSkillInterpreter()->Interpret(mClient, (MessageId)skillCasted, bitstream);
}

void ClientMessageHandler::HandleProjectilePlayerCollision(RakNet::BitStream& bitstream)
{
	int projectileId, playerId;
	bitstream.Read(projectileId);
	bitstream.Read(playerId);

	Player* player = (Player*)mClient->GetWorld()->GetObjectById(playerId);

	char buffer[244];
	sprintf(buffer, "projectile %i collided with player %i health: %.2f\n", projectileId, playerId, player->GetHealth());
	OutputDebugString(buffer);
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
	int startGold, shopTime, roundTime, numRounds, goldPerKill, goldPerWin, lavaDamage, projectileImpulse;
	bitstream.Read(startGold);
	bitstream.Read(shopTime);
	bitstream.Read(roundTime);
	bitstream.Read(numRounds);
	bitstream.Read(goldPerKill);
	bitstream.Read(goldPerWin);
	bitstream.Read(lavaDamage);
	bitstream.Read(projectileImpulse);

	// Format and add it to the chat.
	char buffer[2048];
	sprintf(buffer, "-startgold: %i\n-shoptime: %i\n-roundtime: %i\n-rounds: %i\n-goldperkill: %i\n-goldperwin: %i\n-lavadmg: %f\n-impulse: %f\n",
		startGold, shopTime, roundTime, numRounds, goldPerKill, goldPerWin, lavaDamage, lavaDamage);

	mClient->AddChatText("Cvar list:\n", RGB(0, 0, 255));
	mClient->AddChatText(buffer, RGB(0, 0, 0));
}

void ClientMessageHandler::HandleCvarChange(RakNet::BitStream& bitstream)
{
	char temp[128];
	int value;

	bitstream.Read(temp);
	bitstream.Read(value);

	string cvar = string(temp).substr(1, string(temp).size()-1);

	// Set status text.
	char buffer[244];
	sprintf(buffer, "%s changed to %i\n", cvar.c_str(), value);
	mClient->SetStatusText(buffer, 4);

	// Add to chat.
	mClient->AddChatText(buffer, RGB(0, 200, 0));
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