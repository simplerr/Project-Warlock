#include "LobbyState.h"
#include "NetworkMessages.h"
#include "BitStream.h"
#include "Graphics.h"
#include "Button.h"
#include "Input.h"
#include "PlayingState.h"
#include "ControlManager.h"
#include "TextMenu.h"
#include "Label.h"
#include "Client.h"
#include "Player.h"
#include "Chat.h"
#include <vector>

LobbyState LobbyState::mLobbyState;

void LobbyState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("ui_layout.lua");
	mClient = new Client();
}

void LobbyState::Cleanup(void)
{

}

void LobbyState::Pause()
{

}

void LobbyState::Resume()
{

}

void LobbyState::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(mClient != nullptr)
		mClient->MsgProc(msg, wParam, lParam);
}

void LobbyState::Update(GLib::Input* pInput, double dt)
{
	mControlManager->Update(pInput, dt);
	mClient->ListenForPackets();
}

void LobbyState::Draw(GLib::Graphics* pGraphics)
{
	if(mClient->GetName() == mServerData.host)
		pGraphics->DrawText("HOST!", 100, 100, 20, GLib::ColorRGBA(255, 0, 0, 255));

	vector<Player*> playerList = mClient->GetPlayerList();

	for(int i = 0; i < playerList.size(); i++)
		pGraphics->DrawText(playerList[i]->GetName(), 400, 400 + i*30, 40);

	mControlManager->Draw(pGraphics);
}

void LobbyState::BuildUi()
{
	Label* title = new Label(400, 50, "BrowsingTitle", "Game lobby");
	mControlManager->AddControl(title);

	Label* serverName = new Label(400, 100, "ServerName", mServerData.name);
	mControlManager->AddControl(serverName);

	Button* startButton = new Button(800, 600, "StartGameButton", "Start game");
	startButton->AddPressedListener(&LobbyState::ButtonPressed, this);
	startButton->SetDisabled(!mIsHost);
	mControlManager->AddControl(startButton);

	Button* leaveButton = new Button(800, 600, "LeaveLobbyButton", "Leave lobby");
	leaveButton->AddPressedListener(&LobbyState::ButtonPressed, this);
	mControlManager->AddControl(leaveButton);

	mControlManager->LoadLuaProperties();
}

void LobbyState::SetServerData(ServerData data)
{
	mServerData = data;
	mClient->ConnectToServer(data.localIp);
	mClient->GetChat()->SetDimensions(300, 550, 600, 250);

	mIsHost = mClient->GetName() == mServerData.host ? true : false;

	// [NOTE][HACK]
	BuildUi();
}

void LobbyState::ButtonPressed(Button* pButton)
{
	// Start game.
	if(pButton->GetName() == "StartGameButton")
	{
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)NMSG_START_COUNTDOWN);
		mClient->SendServerMessage(bitstream);
		pButton->SetDisabled(true);
	}
}