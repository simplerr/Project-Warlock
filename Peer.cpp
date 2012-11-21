#include "Peer.h"
#include "Input.h"
#include "Arena.h"
#include "BitStream.h"
#include "World.h"
#include "Object3D.h"
#include "LivingObject.h"
#include "MessageIdentifiers.h"
#include <fstream>

Peer::Peer()
{
	// Create the RakNet peer
	mRaknetPeer = RakNet::RakPeerInterface::GetInstance();

	mArena = new Arena();
	mArena->Init();

	std::ifstream fin("config.txt");
	string ip;
	fin >> mName;
	fin >> ip;
	ConnectToServer(ip);
	fin.close();
}

Peer::~Peer()
{
	delete mArena;

	// Tell the opponent that you left
	/*RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_LEFT_GAME);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);*/

	mRaknetPeer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(mRaknetPeer);
}

void Peer::Update(GLib::Input* pInput, float dt)
{
	ListenForPackets();
	mArena->Update(pInput, this, dt);
}

void Peer::Draw(GLib::Graphics* pGraphics)
{
	mArena->Draw(pGraphics);
}

bool Peer::ConnectToServer(string ip)
{
	if(mRaknetPeer->Startup(1, &RakNet::SocketDescriptor(), 1) == RakNet::RAKNET_STARTED)		{
		if(mRaknetPeer->Connect(ip.c_str(), 27020, NULL, NULL) == RakNet::CONNECTION_ATTEMPT_STARTED)		{
			mRunningServer = false;
			return true;
		}
	}

	return false;
}

bool Peer::ListenForPackets()
{
	RakNet::Packet *packet = nullptr;
	packet = mRaknetPeer->Receive();

	if(packet != nullptr)	{
		HandlePacket(packet);
		mRaknetPeer->DeallocatePacket(packet);
	}

	return true;
}

bool Peer::HandlePacket(RakNet::Packet* pPacket)
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
	case TARGET_ADDED:
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

			GLib::ObjectList* objects = mArena->GetWorld()->GetObjects();
			for(int i = 0; i < objects->size(); i++)
			{
				LivingObject* object = (LivingObject*)objects->operator[](i);
				if(object->GetId() == id)
					object->AddTarget(XMFLOAT3(x, y, z), clear);
			}

			break;
		}
	case TARGET_ADDED_RECEIVED:
			//MessageBox(0, "received!", 0, 0);
		break;
	}

	return true;
}

// Sent from client when a target is added.
void Peer::SendAddTarget(int id, XMFLOAT3 pos, bool clear)
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)TARGET_ADDED);
	bitstream.Write(mName.c_str());
	bitstream.Write((unsigned char)id);
	bitstream.Write(pos.x);
	bitstream.Write(pos.y);
	bitstream.Write(pos.z);
	bitstream.Write(clear);
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

RakNet::RakPeerInterface* Peer::GetRaknetPeer()
{
	return mRaknetPeer;
}

bool Peer::IsHost()
{
	return mRunningServer;
}