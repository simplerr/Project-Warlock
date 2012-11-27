#include "Client.h"
#include "Input.h"
#include "Arena.h"
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
#include <fstream>

Client::Client()
{
	// Create the RakNet peer
	mRaknetPeer		= RakNet::RakPeerInterface::GetInstance();
	mSelectedObject = nullptr;
	mPlayer			= nullptr;

	// Create the world.
	mWorld = new GLib::World();
	mWorld->Init(GLib::GetGraphics());

	mSkillInterpreter = new ClientSkillInterpreter();

	// Connect the graphics light list.
	GLib::GetGraphics()->SetLightList(mWorld->GetLights());

	// Read the name and ip to connect to.
	std::ifstream fin("config.txt");
	string ip;
	fin >> mName;
	fin >> ip;
	ConnectToServer(ip);
	fin.close();
}

Client::~Client()
{
	delete mWorld;
	delete mSkillInterpreter;

	// Tell the opponent that you left
	/*RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_LEFT_GAME);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);*/

	mRaknetPeer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(mRaknetPeer);
}

void Client::Update(GLib::Input* pInput, float dt)
{
	mWorld->Update(dt);

	// If the selected object is the player then poll for action.
	if(mSelectedObject != nullptr && mSelectedObject->GetId() == mPlayer->GetId()) 
		mPlayer->PollAction(this, pInput);

	// Poll for object selection.
	PollSelection(pInput);

	if(pInput->KeyPressed('C'))
		RequestClientNames();

	// Listen for incoming packets.
	ListenForPackets();
}

void Client::Draw(GLib::Graphics* pGraphics)
{
	// Draw the world.
	mWorld->Draw(pGraphics);
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

	// Read the Id.
	bitstream.Read(packetID);

	switch(packetID)
	{
	case ID_NEW_INCOMING_CONNECTION:
			break;
	case NMSG_WORLD_UPDATE:
		{
			unsigned char id;
			XMFLOAT3 pos;
			bitstream.Read(id);
			bitstream.Read(pos.x);
			bitstream.Read(pos.y);
			bitstream.Read(pos.z);

			GLib::ObjectList* objects = mWorld->GetObjects();
			for(int i = 0; i < objects->size(); i++)
			{
				GLib::Object3D* object = objects->operator[](i);

				// Do interpolation stuff here.
				if(object->GetId() == id)
					object->SetPosition(pos);
			}
		}
		break;
	case NMSG_TARGET_ADDED:
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

			break;
		}
	case NMSG_PROJECTILE_ADDED:
		{
			int owner, id;
			XMFLOAT3 startPos, direction;
			bitstream.Read(owner);
			bitstream.Read(startPos);
			bitstream.Read(direction);
			bitstream.Read(id);

			Projectile* projectile = new Projectile(owner, startPos, direction);
			mWorld->AddObject(projectile);
			projectile->SetId(id);
		}
		break;
	case NMSG_OBJECT_REMOVED:
		{
			int id;
			bitstream.Read(id);
			mWorld->RemoveObject(id);
		}
		break;
	// Sent when the server has received the client connection.
	case NMSG_CONNECTION_SUCCESS:
		{
			// Add the players already connected.
			char name[244];
			int numPlayers, id;
			XMFLOAT3 pos;

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
			}

			// Send the client info to the server (name etc).
			RakNet::BitStream bitstream;
			bitstream.Write((unsigned char)NMSG_CLIENT_CONNECTION_DATA);
			bitstream.Write(mName.c_str());
			mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

		}
		break;
	case NMSG_ADD_PLAYER:
		{
			char buffer[244];
			int id;
			bitstream.Read(buffer);
			bitstream.Read(id);

			string name = buffer;

			//Sleep(10000);

			// Add a new player to the World.
			Player* player = new Player();
			player->SetName(name);
			player->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));	// [NOTE]
			mWorld->AddObject(player);
			player->SetId(id);	// Use the servers ID.

			if(mName == name && mPlayer == nullptr) {
				OutputDebugString(string("Successfully connected to the server! " + name + "\n").c_str());
				mPlayer = player;
			}
			else
				OutputDebugString(string(name + " has connected!\n").c_str());

		}
		break;
	// A player has diconnected.
	// [NOTE] The removal from the World happens in NMSG_OBJECT_REMOVED.
	case NMSG_PLAYER_DISCONNECTED:
		{
			char buffer[244];
			bitstream.Read(buffer);

			string name = buffer;
			OutputDebugString(string(name + " has disconnected!").c_str());
		}
		break;
	case NSMG_CONNECTED_CLIENTS:
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
		break;
	case NMSG_SKILL_CAST:
		{
			unsigned char skillCasted;
			bitstream.Read(skillCasted);
			mSkillInterpreter->Interpret(this, (MessageId)skillCasted, bitstream);
		}
		break;
	case NMSG_PROJECTILE_PLAYER_COLLISION:
		{
			int projectileId, playerId;
			bitstream.Read(projectileId);
			bitstream.Read(playerId);

			char buffer[244];
			sprintf(buffer, "projectile %i collided with player %i\n", projectileId, playerId);
			OutputDebugString(buffer);
		}
		break;
	}

	return true;
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

void Client::SendAddProjectile(int owner, XMFLOAT3 startPos, XMFLOAT3 endPos)
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_PROJECTILE_ADDED);
	bitstream.Write(owner);
	XMFLOAT3 dir = endPos - startPos;
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));
	bitstream.Write(startPos);
	bitstream.Write(dir);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::PollSelection(GLib::Input* pInput)
{
	// Get the selected object.
	if(pInput->KeyPressed(VK_LBUTTON))
	{
		Player* selected = (Player*)mWorld->GetSelectedObject(pInput->GetWorldPickingRay());
		if(selected != nullptr) 
		{
			if(mSelectedObject != nullptr) {
				mSelectedObject->SetSelected(false);
				mSelectedObject->SetMaterials(GLib::Material(GLib::Colors::White));
			}

			mSelectedObject = selected;
			mSelectedObject->SetSelected(true);
			mSelectedObject->SetMaterials(GLib::Material(XMFLOAT4(1.0f, 127.0f/255.0f, 38/255.0f, 0.12f) * 4));
		}
		//else if(mSelectedObject != nullptr) // Fire projectile.
		//{
		//	XMFLOAT3 pos = mWorld->GetTerrainIntersectPoint(pInput->GetWorldPickingRay());

		//	if(pos.x != numeric_limits<float>::infinity())
		//		SendAddProjectile(mSelectedObject->GetId(), mSelectedObject->GetPosition(), pos);
		//}
	}
}

void Client::RequestClientNames()
{
	RakNet::BitStream bitstream;

	bitstream.Write((unsigned char)NMSG_REQUEST_CLIENT_NAMES);
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