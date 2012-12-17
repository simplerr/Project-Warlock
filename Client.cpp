#include "Client.h"
#include "ClientMessageHandler.h"
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
#include "ClientArena.h"

Client::Client()
{
	// Create the RakNet peer
	mRaknetPeer		  = RakNet::RakPeerInterface::GetInstance();

	mArena			  = new ClientArena(this);
	mSkillInterpreter = new ClientSkillInterpreter();
	mMessageHandler	  = new ClientMessageHandler(this);
	mUserInterface    = new UserInterface(this);
	mStatusText       = new GLib::StatusText("Test", 600, 200, 6);

	//// Read the name and ip to connect to.
	//std::ifstream fin("config.txt");
	//string ip;
	//fin >> mName;
	//fin >> ip;
	//fin.close();

	//ConnectToServer(ip);

	InitShoppingState(mArenaState);

	mRoundEnded = false;
}

Client::~Client()
{
	delete mSkillInterpreter;
	delete mMessageHandler;
	delete mUserInterface;
	delete mStatusText;
	delete mArena;

	mRaknetPeer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(mRaknetPeer);
}

void Client::Update(GLib::Input* pInput, float dt)
{
	mArena->Update(pInput, dt);
	mStatusText->Update(dt);
	mUserInterface->Update(pInput, dt);

	// Testing..
	if(pInput->KeyPressed('C'))
		RequestClientNames();

	// Listen for incoming packets.
	ListenForPackets();
}

void Client::Draw(GLib::Graphics* pGraphics)
{
	mArena->Draw(pGraphics);
	mStatusText->Draw(pGraphics);
	mUserInterface->Draw(pGraphics);

	//DrawScores(pGraphics);

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

	char buffer[128];
	sprintf(buffer, "Timer: %.2f", mArenaState.elapsed);
	pGraphics->DrawText(buffer, 900, 200, 14);
}

bool Client::ConnectToServer(string ip)
{
	// [TODO] If this returns true it doesn't mean that the connection was successful.
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
			mMessageHandler->HandleWorldUpdate(bitstream);
			break;
		case NMSG_TARGET_ADDED:
			mMessageHandler->HandleTargetAdded(bitstream);
			break;
		case NMSG_OBJECT_REMOVED:
			mMessageHandler->HandleObjectRemvoed(bitstream);
			break;
		case NMSG_CONNECTION_SUCCESS:
			mMessageHandler->HandleConnectionSuccess(bitstream);
			break;
		case NMSG_ADD_PLAYER:
			mMessageHandler->HandleAddPlayer(bitstream);
			break;
		case NMSG_PLAYER_DISCONNECTED:
			mMessageHandler->HandlePlayerDisconnected(bitstream);
			break;
		case NSMG_CONNECTED_CLIENTS:
			mMessageHandler->HandleGetConnectedPlayers(bitstream);
			break;
		case NMSG_SKILL_CAST:
			mMessageHandler->HandleSkillCasted(bitstream);
			break;
		case NMSG_PROJECTILE_PLAYER_COLLISION:
			mMessageHandler->HandleProjectilePlayerCollision(bitstream);
			break;
		case NMSG_ITEM_ADDED:
			{
				int playerId;
				bitstream.Read(playerId);
				mUserInterface->HandleItemAdded((Player*)mArena->GetWorld()->GetObjectById(playerId), bitstream);
				break;
			}
		case NMSG_ITEM_REMOVED:
			{
				int playerId;
				bitstream.Read(playerId);
				mUserInterface->HandleItemRemoved((Player*)mArena->GetWorld()->GetObjectById(playerId), bitstream);
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
			mMessageHandler->HandleRoundStarted(bitstream);
			break;
		case NMSG_ROUND_ENDED:
			mMessageHandler->HandleRoundEnded(bitstream);
			break;
		case NMSG_CHAT_MESSAGE_SENT:
			mUserInterface->HandleChatMessage(bitstream);
			break;
		case NMSG_REQUEST_CVAR_LIST:
			mMessageHandler->HandleCvarList(bitstream);
			break;
		case NMSG_CVAR_CHANGE:
			mMessageHandler->HandleCvarChange(bitstream);
			break;
		case NMSG_PLAYER_ELIMINATED:
			mMessageHandler->HandlePlayerEliminated(bitstream);
			break;
		case NMSG_ADD_CHAT_TEXT:
			mUserInterface->HandleAddChatText(bitstream);
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

void Client::StartRound()
{
	mArena->ResetPlayers();
	InitShoppingState(mArenaState);
	AddChatText("Round started! Shopping time!\n", RGB(0, 200, 0));
	mRoundEnded = false;
}

void Client::EndRound(string winner)
{
	mWinner = winner;
	mScoreMap[mWinner]++;
	mRoundEnded = true;
}

void Client::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	mUserInterface->MsgProc(msg, wParam, lParam);
}

RakNet::RakPeerInterface* Client::GetRaknetPeer()
{
	return mRaknetPeer;
}

int Client::GetLocalPlayerId()
{
	return mArena->GetLocalPlayer()->GetId();
}

GLib::World* Client::GetWorld()
{
	return mArena->GetWorld();
}

bool Client::IsLocalPlayerSelected()
{
	return mArena->IsLocalPlayerSelected();
}

Player* Client::GetLocalPlayer()
{
	return mArena->GetLocalPlayer();
}

CurrentState Client::GetArenaState()
{
	return mArenaState.state;
}

string Client::GetName()
{
	return mName;
}

ClientArena* Client::GetArena()
{
	return mArena;
}

void Client::SetArenaState(CurrentState state)
{
	mArenaState.state = state;
}

void Client::SetScore(string name, int score)
{
	mScoreMap[name] = score;
}

void Client::SetLocalPlayer(Player* pPlayer)
{
	mArena->SetLocalPlayer(pPlayer);
}

void Client::SetSelectedPlayer(Player* pPlayer)
{
	mArena->SetSelectedPlayer(pPlayer);
}

Chat* Client::GetChat()
{
	return mUserInterface->GetChat();
}

ClientSkillInterpreter* Client::GetSkillInterpreter()
{
	return mSkillInterpreter;
}

void Client::AddChatText(string text, COLORREF color)
{
	mUserInterface->GetChat()->AddText((char*)text.c_str(), color);
}

void Client::SetStatusText(string text, float duration)
{
	mStatusText->SetText(text, duration);
}

UserInterface* Client::GetUi()
{
	return mUserInterface;
}