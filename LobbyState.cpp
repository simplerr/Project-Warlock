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
#include "UserInterface.h"

LobbyState LobbyState::mLobbyState;

void LobbyState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mControlManager = new ControlManager("ui_layout.lua");
	mClient = new Client();

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/menu_bkgd.png");
	mPlayerListBkgd = GLib::GetGraphics()->LoadTexture("textures/white_transparent.png");
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
	pGraphics->DrawScreenQuad(mBkgdTexture, GLib::GetClientWidth()/2, GLib::GetClientHeight()/2, GLib::GetClientWidth(), GLib::GetClientHeight());
	pGraphics->DrawScreenQuad(mPlayerListBkgd, GLib::GetClientWidth()/2, 300, 300, 400);

	if(mClient->GetName() == mServerData.host)
		pGraphics->DrawText("HOST!", 100, 100, 20, GLib::ColorRGBA(255, 0, 0, 255));

	vector<Player*> playerList = mClient->GetPlayerList();

	for(int i = 0; i < playerList.size(); i++) {
		GLib::Rect textSize = pGraphics->MeasureText(playerList[i]->GetName(), 40, "Arial");
		pGraphics->DrawText(playerList[i]->GetName(), GLib::GetClientWidth()/2-textSize.right/2, 100 + i*40, 40);
	}

	mControlManager->Draw(pGraphics);
}

void LobbyState::BuildUi()
{
	Label* serverName = new Label(800, 100, "StateHeader", mServerData.name);
	serverName->SetAlignment(false, true);
	mControlManager->AddControl(serverName);

	Button* startButton = new Button(800, 600, "StartGameButton", "Start game");
	startButton->AddPressedListener(&LobbyState::ButtonPressed, this);
	startButton->SetDisabled(!mIsHost);
	startButton->SetAlignment(false, true);
	mControlManager->AddControl(startButton);

	Button* leaveButton = new Button(800, 600, "LeaveLobbyButton", "Leave lobby");
	leaveButton->AddPressedListener(&LobbyState::ButtonPressed, this);
	leaveButton->SetAlignment(false, true);
	mControlManager->AddControl(leaveButton);

	mControlManager->LoadLuaProperties();
}

void LobbyState::SetServerData(ServerData data)
{
	mServerData = data;
	mClient->ConnectToServer(data.localIp);
	float chatWidth = 600;
	float chatHeight = 250;
	mClient->GetChat()->SetDimensions(GLib::GetClientWidth()/2-chatWidth/2, GLib::GetClientHeight() - chatHeight - 100, chatWidth, chatHeight);

	mIsHost = mClient->GetName() == mServerData.host ? true : false;

	// [NOTE][HACK]
	BuildUi();
	OnResize(GLib::GetClientWidth(), GLib::GetClientHeight());
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

void LobbyState::StartGame()
{
	//ChangeState(PlayingState::Instance());
}

void LobbyState::OnResize(float width, float height)
{
	mControlManager->OnResize(width, height);
	mControlManager->LoadLuaProperties();
}