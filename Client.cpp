#include "Client.h"
#include "ServerCvars.h"
#include "Input.h"
#include "BitStream.h"
#include "World.h"
#include "Object3D.h"
#include "Actor.h"
#include "MessageIdentifiers.h"
#include "Projectile.h"
#include "Graphics.h"
#include "Player.h"
#include "NetworkMessages.h"
#include "ClientSkillInterpreter.h"
#include "ItemLoaderXML.h"
#include "Inventory.h"
#include "Shop.h"
#include <fstream>
#include "UserInterface.h"
#include "Chat.h"
#include "StatusText.h"

Client::Client()
{
	// Create the RakNet peer
	mRaknetPeer		= RakNet::RakPeerInterface::GetInstance();
	mSelectedPlayer = nullptr;
	mPlayer			= nullptr;

	// Create the world.
	mWorld = new GLib::World();
	mWorld->Init(GLib::GetGraphics());

	mWorld->AddObjectAddedListener(&Client::OnObjectAdded, this);
	mWorld->AddObjectRemovedListener(&Client::OnObjectRemoved, this);

	mSkillInterpreter = new ClientSkillInterpreter();
	mUserInterface = new UserInterface(this);
	mStatusText = new GLib::StatusText("Test", 600, 200, 6);

	// Connect the graphics light list.
	GLib::GetGraphics()->SetLightList(mWorld->GetLights());

	// Read the name and ip to connect to.
	std::ifstream fin("config.txt");
	string ip;
	fin >> mName;
	fin >> ip;
	ConnectToServer(ip);
	fin.close();

	InitShoppingState(mArenaState);

	mRoundEnded = false;
}

Client::~Client()
{
	delete mWorld;
	delete mSkillInterpreter;
	delete mUserInterface;
	delete mStatusText;

	// Tell the opponent that you left
	/*RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_LEFT_GAME);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);*/

	mRaknetPeer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(mRaknetPeer);
}

void Client::Update(GLib::Input* pInput, float dt)
{
	// Update the world.
	mWorld->Update(dt);
	mStatusText->Update(dt);
	mUserInterface->Update(pInput, dt);

	// Poll for object selection.
	PollSelection(pInput);

	// If the selected object is the player then poll for action.
	if(mSelectedPlayer != nullptr && mSelectedPlayer->GetId() == mPlayer->GetId() && !mUserInterface->PointInsideUi(pInput->MousePosition())) 
		mPlayer->PollAction(this, pInput);

	// Testing..
	if(pInput->KeyPressed('C'))
		RequestClientNames();

	// Listen for incoming packets.
	ListenForPackets();
}

void Client::Draw(GLib::Graphics* pGraphics)
{
	// Draw the world.
	mWorld->Draw(pGraphics);
	mStatusText->Draw(pGraphics);
	mUserInterface->Draw(pGraphics);

	if(mSelectedPlayer != nullptr) {
		char buffer[244];
		sprintf(buffer, "Health: %.2f\nRegen: %.2f\nMs: %.2f\nKnockbak res: %.2f\nLava Immunity: %.2f\nDamage: %.2f\nLifesteal: %.2f\n\nGold: %i\n\nTimer: %.2f", mSelectedPlayer->GetHealth(), mSelectedPlayer->GetRegen(), mSelectedPlayer->GetMovementSpeed(),
			mSelectedPlayer->GetKnockBackResistance(), mSelectedPlayer->GetLavaImmunity(), mSelectedPlayer->GetDamage(), mSelectedPlayer->GetLifeSteal(), mSelectedPlayer->GetGold(), mArenaState.elapsed);
		pGraphics->DrawText(buffer, 10, 10, 16, 0xff000000);
	}

	DrawScores(pGraphics);

	if(mRoundEnded)
		pGraphics->DrawText(mWinner + " won the round!", 600, 200, 20);
}

void Client::DrawScores(GLib::Graphics* pGraphics)
{
	string scoreList;
	for(auto iter = mScoreMap.begin(); iter != mScoreMap.end(); iter++)
	{
		char score[10];
		sprintf(score, "%i", (*iter).second);
		scoreList += (*iter).first + ": " + score + "\n";
	}

	pGraphics->DrawText(scoreList, 900, 100, 14);
}

bool Client::ConnectToServer(string ip)
{
	if(mRaknetPeer->Startup(1, &RakNet::SocketDescriptor(), 1) == RakNet::RAKNET_STARTED)		{
		if(mRaknetPeer->Connect(ip.c_str(), 27020, NULL, NULL) == RakNet::CONNECTION_ATTEMPT_STARTED)		
			return true;
	}

	return false;
}

bool Client::ListenForPackets()
{
	RakNet::Packet *packet = nullptr;
	
	packet = mRaknetPeer->Receive();

	while(packet != nullptr)
	{
		HandlePacket(packet);
		mRaknetPeer->DeallocatePacket(packet);
		packet = mRaknetPeer->Receive();
	}

	return true;
}

bool Client::HandlePacket(RakNet::Packet* pPacket)
{
	// Receive the packet.
	RakNet::BitStream bitstream((unsigned char*)pPacket->data, pPacket->length, false);
	unsigned char packetID;

	// Read the packet id.
	bitstream.Read(packetID);

	// Switch the packet id.
	switch(packetID)
	{
		case NMSG_WORLD_UPDATE:
			HandleWorldUpdate(bitstream);
			break;
		case NMSG_TARGET_ADDED:
			HandleTargetAdded(bitstream);
			break;
		case NMSG_OBJECT_REMOVED:
			HandleObjectRemvoed(bitstream);
			break;
		case NMSG_CONNECTION_SUCCESS:
			HandleConnectionSuccess(bitstream);
			break;
		case NMSG_ADD_PLAYER:
			HandleAddPlayer(bitstream);
			break;
		case NMSG_PLAYER_DISCONNECTED:
			HandlePlayerDisconnected(bitstream);
			break;
		case NSMG_CONNECTED_CLIENTS:
			HandleGetConnectedPlayers(bitstream);
			break;
		case NMSG_SKILL_CAST:
			HandleSkillCasted(bitstream);
			break;
		case NMSG_PROJECTILE_PLAYER_COLLISION:
			HandleProjectilePlayerCollision(bitstream);
			break;
		case NMSG_ITEM_ADDED:
			{
				int playerId;
				bitstream.Read(playerId);
				mUserInterface->HandleItemAdded((Player*)mWorld->GetObjectById(playerId), bitstream);
				break;
			}
		case NMSG_ITEM_REMOVED:
			{
				int playerId;
				bitstream.Read(playerId);
				mUserInterface->HandleItemRemoved((Player*)mWorld->GetObjectById(playerId), bitstream);
				break;
			}
		case NMSG_STATE_TIMER:
			bitstream.Read(mArenaState.elapsed);
			break;
		case NMSG_CHANGETO_PLAYING:
			InitPlayingState(mArenaState);
			break;
		case NMSG_CHANGETO_SHOPPING:
			InitShoppingState(mArenaState);
			break;
		case NMSG_ROUND_START:
			HandleRoundStarted(bitstream);
			break;
		case NMSG_ROUND_ENDED:
			HandleRoundEnded(bitstream);
			break;
		case NMSG_CHAT_MESSAGE_SENT:
			mUserInterface->HandleChatMessage(bitstream);
			break;
		case NMSG_REQUEST_CVAR_LIST:
			HandleCvarList(bitstream);
			break;
		case NMSG_CVAR_CHANGE:
			HandleCvarChange(bitstream);
			break;
		case NMSG_PLAYER_ELIMINATED:
			HandlePlayerEliminated(bitstream);
			break;
	}

	return true;
}

void Client::HandleWorldUpdate(RakNet::BitStream& bitstream)
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

	GLib::ObjectList* objects = mWorld->GetObjects();
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

void Client::HandleTargetAdded(RakNet::BitStream& bitstream)
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

	GLib::ObjectList* objects = mWorld->GetObjects();
	for(int i = 0; i < objects->size(); i++)
	{
		Actor* object = (Actor*)objects->operator[](i);
		if(object->GetId() == id)
			object->AddTarget(XMFLOAT3(x, y, z), clear);
	}
}

void Client::HandleObjectRemvoed(RakNet::BitStream& bitstream)
{
	int id;
	bitstream.Read(id);
	mWorld->RemoveObject(id);
}

void Client::HandleConnectionSuccess(RakNet::BitStream& bitstream)
{
	// Add the players already connected.
	char name[244];
	int numPlayers, id;
	XMFLOAT3 pos;

	bitstream.Read(mArenaState.state);
	bitstream.Read(numPlayers);

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
		mWorld->AddObject(player);
		player->SetId(id);	// Use the servers ID.

		mScoreMap[name] = 0;
	}

	// Send the client info to the server (name etc).
	RakNet::BitStream sendBitstream;
	sendBitstream.Write((unsigned char)NMSG_CLIENT_CONNECTION_DATA);
	sendBitstream.Write(mName.c_str());
	mRaknetPeer->Send(&sendBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::HandleAddPlayer(RakNet::BitStream& bitstream)
{
	char buffer[244];
	int id, gold;
	bitstream.Read(buffer);
	bitstream.Read(id);
	bitstream.Read(gold);

	string name = buffer;

	mScoreMap[name] = 0;

	// Add a new player to the World.
	Player* player = new Player();
	player->SetName(name);
	player->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));	// [NOTE]
	mWorld->AddObject(player);
	player->SetId(id);	// Use the servers ID.
	player->SetGold(gold);

	if(mName == name && mPlayer == nullptr) {
		OutputDebugString(string("Successfully connected to the server! " + name + "\n").c_str());
		mPlayer = player;
		mPlayer->SetLocalPlayer(true);	// Draws a square over the local players head.

		mSelectedPlayer = mPlayer;
		mSelectedPlayer->SetSelected(true);
		mSelectedPlayer->SetMaterials(GLib::Material(XMFLOAT4(1.0f, 127.0f/255.0f, 38/255.0f, 0.12f) * 4));

		// [NOTEOTOEOTOE]
		mUserInterface->SetSelectedPlayer(mPlayer);
	}
	else
		OutputDebugString(string(name + " has connected!\n").c_str());
}

void Client::HandlePlayerDisconnected(RakNet::BitStream& bitstream)
{
	char buffer[244];
	bitstream.Read(buffer);

	string name = buffer;
	OutputDebugString(string(name + " has disconnected!").c_str());
}

void Client::HandleGetConnectedPlayers(RakNet::BitStream& bitstream)
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

	OutputDebugString(string("Users connected:\n").c_str());
	for(int i = 0; i < numConnected; i++)
		OutputDebugString(string(names[i] + "\n").c_str());
}

void Client::HandleSkillCasted(RakNet::BitStream& bitstream)
{
	unsigned char skillCasted;
	bitstream.Read(skillCasted);
	mSkillInterpreter->Interpret(this, (MessageId)skillCasted, bitstream);
}

void Client::HandleProjectilePlayerCollision(RakNet::BitStream& bitstream)
{
	int projectileId, playerId;
	bitstream.Read(projectileId);
	bitstream.Read(playerId);

	Player* player = (Player*)mWorld->GetObjectById(playerId);

	char buffer[244];
	sprintf(buffer, "projectile %i collided with player %i health: %.2f\n", projectileId, playerId, player->GetHealth());
	OutputDebugString(buffer);
}

void Client::HandleRoundStarted(RakNet::BitStream& bitstream)
{
	// [NOTE] The position is set by the server. Then passed by NMSG_UPDATEW_WORLD.
	for(int i = 0; i < mPlayerList.size(); i++) 
	{
		mPlayerList[i]->SetEliminated(false);
		mPlayerList[i]->SetHealth(100);	// [NOTE][HACK]
	}

	InitShoppingState(mArenaState);

	OutputDebugString("Round started! Shopping time!");

	mRoundEnded = false;
}

void Client::HandleRoundEnded(RakNet::BitStream& bitstream)
{
	char buffer[244];
	bitstream.Read(buffer);
	mWinner = buffer;

	// Increment the winners score.
	mScoreMap[mWinner]++;

	mRoundEnded = true;
}

void Client::HandleCvarList(RakNet::BitStream& bitstream)
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
	sprintf(buffer, "startGold: %i\nshopTime: %i\nroundTime: %i\nnrounds: %i\ngoldPerKill: %i\ngoldPerWin: %i\nlavaDamage: %f\nprojectileImpulse: %f\n",
		startGold, shopTime, roundTime, numRounds, goldPerKill, goldPerWin, lavaDamage, lavaDamage);

	Chat* chat = mUserInterface->GetChat();
	chat->AddText(buffer, RGB(0, 0, 0));
}

void Client::HandleCvarChange(RakNet::BitStream& bitstream)
{
	char temp[128];
	int value;

	bitstream.Read(temp);
	bitstream.Read(value);

	string cvar = string(temp).substr(1, string(temp).size()-1);

	char buffer[244];
	sprintf(buffer, "%s changed to %i", cvar.c_str(), value);
	mStatusText->SetText(buffer, 4);
}

void Client::HandlePlayerEliminated(RakNet::BitStream& bitstream)
{
	char killed[128];
	char eliminator[128];

	bitstream.Read(killed);
	bitstream.Read(eliminator);

	string chatText = string(killed) + " was pwnd by " + string(eliminator) + "!\n";
	mUserInterface->GetChat()->AddText((char*)chatText.c_str(), RGB(255, 0, 0));
}

void Client::SendServerMessage(RakNet::BitStream& bitstream)
{
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

// Sent from client when a target is added.
void Client::SendAddTarget(int id, XMFLOAT3 pos, bool clear)
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_TARGET_ADDED);
	bitstream.Write(mName.c_str());
	bitstream.Write((unsigned char)id);
	bitstream.Write(pos.x);
	bitstream.Write(pos.y);
	bitstream.Write(pos.z);
	bitstream.Write(clear);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::PollSelection(GLib::Input* pInput)
{
	// Get the selected object.
	if(pInput->KeyPressed(VK_LBUTTON) && !mPlayer->IsCastingSkill() && !mUserInterface->PointInsideUi(pInput->MousePosition()))
	{
		Player* selected = (Player*)mWorld->GetSelectedObject(pInput->GetWorldPickingRay(), GLib::PLAYER);
		SetSelectedPlayer(selected);
	}
}

void Client::SetSelectedPlayer(Player* pPlayer)
{
	if(pPlayer != nullptr) 
	{
		if(mSelectedPlayer != nullptr) {
			mSelectedPlayer->SetSelected(false);
			mSelectedPlayer->SetMaterials(GLib::Material(GLib::Colors::White));
		}

		mSelectedPlayer = pPlayer;
		mSelectedPlayer->SetSelected(true);
		mSelectedPlayer->SetMaterials(GLib::Material(XMFLOAT4(1.0f, 127.0f/255.0f, 38/255.0f, 0.12f) * 4));
		mUserInterface->SetSelectedPlayer(mSelectedPlayer);
	}
}

void Client::RequestClientNames()
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_REQUEST_CLIENT_NAMES);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::RequestCvarList()
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_REQUEST_CVAR_LIST);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

RakNet::RakPeerInterface* Client::GetRaknetPeer()
{
	return mRaknetPeer;
}

int Client::GetPlayerId()
{
	return mPlayer->GetId();
}

GLib::World* Client::GetWorld()
{
	return mWorld;
}

bool Client::IsLocalPlayerSelected()
{
	return (mSelectedPlayer != nullptr && mSelectedPlayer->GetId() == mPlayer->GetId());
}

//! Gets called in World::AddObject().
void Client::OnObjectAdded(GLib::Object3D* pObject)
{
	// Add player to mPlayerList.
	if(pObject->GetType() == GLib::PLAYER)
		mPlayerList.push_back((Player*)pObject);
}

//! Gets called in World::RemoveObject().
void Client::OnObjectRemoved(GLib::Object3D* pObject)
{
	// Remove player from mPlayerList:
	if(pObject->GetType() == GLib::PLAYER) 
		RemovePlayer(pObject->GetId());

	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_OBJECT_REMOVED);
	bitstream.Write(pObject->GetId());
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

//! Removes a player from mPlayerList.
void Client::RemovePlayer(int id)
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

void Client::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	mUserInterface->MsgProc(msg, wParam, lParam);
}

Player* Client::GetPlayer()
{
	return mPlayer;
}

GameState Client::GetArenaState()
{
	return mArenaState.state;
}

string Client::GetName()
{
	return mName;
}