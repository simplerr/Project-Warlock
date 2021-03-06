#include "Client.h"
#include "PlayerModule.h"
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
#include "LobbyState.h"
#include "PlayingState.h"
#include "RoundHandler.h"
#include "Sound.h"
#include "Effects.h"

Client::Client()
{
	// Create the RakNet peer
	mRaknetPeer		  = RakNet::RakPeerInterface::GetInstance();

	mArena			  = new ClientArena(this);
	mMessageHandler	  = new ClientMessageHandler(this);
	mUserInterface    = new UserInterface(this);
	mRoundHandler	  = new RoundHandler();

	mUserInterface->SetReady(true);

	//// Read the name and ip to connect to.
	std::ifstream fin("data/config.txt");
	string ip;
	fin >> mName;
	fin >> ip;
	fin.close();

	InitShoppingState(mRoundHandler->GetArenaState(), false);
}

Client::~Client()
{
	delete mMessageHandler;
	delete mUserInterface;
	//delete mArena;
	delete mRoundHandler;

	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_PLAYER_DISCONNECTED);
	SendServerMessage(bitstream);
	Sleep(200);

	mRaknetPeer->Shutdown(0);
	RakNet::RakPeerInterface::DestroyInstance(mRaknetPeer);
}

void Client::Update(GLib::Input* pInput, float dt)
{
	// Listen for incoming packets.
	if(!pInput->KeyDown(VK_SPACE))
		ListenForPackets();

	mArena->Update(pInput, dt);
	mUserInterface->Update(pInput, dt);
}

void Client::Draw(GLib::Graphics* pGraphics)
{
	mArena->Draw(pGraphics);
	mUserInterface->Draw(pGraphics);
	mRoundHandler->Draw(pGraphics);
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
		case NMSG_SERVER_SHUTDOWN:
			mMessageHandler->HandleServerShutdown(bitstream);
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
			gSound->PlayEffect("data/sounds/disconnect.wav");
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
				PlayerModule* module = mArena->GetPlayerModule(playerId);
				mUserInterface->HandleItemAdded(module, bitstream);
				break;
			}
		case NMSG_ITEM_REMOVED:
			{
				int playerId;
				bitstream.Read(playerId);
				PlayerModule* module = mArena->GetPlayerModule(playerId);
				mUserInterface->HandleItemRemoved(module, bitstream);
				break;
			}
		case NMSG_STATE_TIMER:
			bitstream.Read(mRoundHandler->GetArenaState().elapsed);
			break;
		case NMSG_CHANGETO_PLAYING:
			InitPlayingState(mRoundHandler->GetArenaState(), false, GetLocalPlayer()->GetPosition());
			gSound->PlayEffect("data/sounds/shopping_done.wav");
			break;
		case NMSG_CHANGETO_SHOPPING:
			// [NOTE] Server never sends this message
			InitShoppingState(mRoundHandler->GetArenaState(), false);
			gSound->PlayEffect("data/sounds/shopping_start.wav");			
			break;
		case NMSG_ROUND_START:
			mMessageHandler->HandleRoundStarted(bitstream);
			gSound->PlayEffect("data/sounds/shopping_start.wav");			
			break;
		case NMSG_ROUND_ENDED:
			mMessageHandler->HandleRoundEnded(bitstream);
			break;
		case NMSG_GAME_OVER: 
			{
				char winner[244];
				bitstream.Read(winner);
				mRoundHandler->AddScore(winner, 1);
				mUserInterface->DisplayGameOver(this, mRoundHandler->GetGameWinner());
				mArena->RemoveStatusEffects();
				mGameOver = true;
				break;
			}
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
		case NMSG_GAME_STARTED:
			// This is a bit ugly [NOTE].
			LobbyState::Instance()->ChangeState(PlayingState::Instance());
			PlayingState::Instance()->SetClient(this);
			break;
		case NMSG_COUNTDOWN_TICK: {
			char buffer[256], file[64];
			bitstream.Read(buffer);
			mUserInterface->GetChat()->AddText((char*)string(string(buffer) + "\n").c_str(), RGB(0, 0, 0));

			int num = atoi(buffer);
			if(num >= 0 && num <= 3) {
				sprintf(file, "data/sounds/%s.wav", buffer);
				gSound->PlayEffect(file);
			}			
			break;
								  }
		case NMSG_PERFORM_REMATCH:
			mMessageHandler->HandlePerformRematch(bitstream);
			//mUserInterface->GetItemLoader()->ReloadItems();
			mGameOver = false;
			mRoundHandler->ResetScores();
			mUserInterface->RemoveGameOverScreen();
			break;
		case NMSG_FLOOD_START:
			gSound->PlayEffect("data/sounds/flood_start.wav");
			break;
		case NMSG_ARENA_RADIUS:
			float radius;
			bitstream.Read(radius);
			GLib::Effects::TerrainFX->SetArenaRadius(radius);
			break;
		case NMSG_PROJECTILE_PROJECTILE_COLLISION:
			gSound->PlayEffect("data/sounds/fireball_impact.wav");
			break;
	}

	return true;
}

void Client::SendServerMessage(RakNet::BitStream& bitstream)
{
	mRaknetPeer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::StartRound()
{
	mArena->ResetPlayers();
	InitShoppingState(mRoundHandler->GetArenaState(), false);
	AddChatText("Round started! Shopping time!\n", RGB(0, 200, 0));

	// Remove the "PlayerX won the round!" status text.
	mUserInterface->SetStatusText("nothing", 0);

	GLib::Effects::TerrainFX->SetArenaRadius(gCvars->GetCvarValue(Cvars::ARENA_RADIUS));
}

void Client::EndRound(string winner)
{
	mRoundHandler->SetWinner(winner);
	mRoundHandler->AddScore(winner, 1);

	mArena->RemoveStatusEffects();

	// Set the status text.
	mUserInterface->SetStatusText(winner + " won the round!", 1337, 40, GLib::ColorRGBA(255, 0, 0, 255));

	if(mArena->GetLocalPlayer()->GetName() == winner) 
	{
		int id = rand() % 7;

		if(id == 0)
			gSound->PlayEffect("data/sounds/godlike.wav");
		else if(id == 1)
			gSound->PlayEffect("data/sounds/rampage.wav");
		else if(id == 2)
			gSound->PlayEffect("data/sounds/wickedsick.wav");
		else
		{
			// Default sound effect on round over.
			gSound->PlayEffect("data/sounds/humiliation.wav");
		}
	}
}

void Client::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(mUserInterface->IsReady())
		mUserInterface->MsgProc(msg, wParam, lParam);
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
	return mRoundHandler->GetArenaState().state;
}

string Client::GetName()
{
	return mName;
}

void Client::SetSelectedPlayer(Player* pPlayer)
{
	mArena->SetSelectedPlayer(pPlayer);
}

Chat* Client::GetChat()
{
	return mUserInterface->GetChat();
}

void Client::AddChatText(string text, COLORREF color)
{
	mUserInterface->GetChat()->AddText((char*)text.c_str(), color);
}

UserInterface* Client::GetUi()
{
	return mUserInterface;
}

vector<Player*> Client::GetPlayerList()
{
	return mArena->GetPlayerList();
}

RoundHandler* Client::GetRoundHandler()
{
	return mRoundHandler;
}

bool Client::IsGameOver()
{
	return mGameOver;
}

bool Client::IsHost()
{
	return GetName() == mServerData.host;
}

void Client::SetServerData(ServerData serverData)
{
	mServerData = serverData;
}

ClientArena* Client::GetArena()
{
	return mArena;
}