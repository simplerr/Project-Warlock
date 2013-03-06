#include "BrowsingState.h"
#include "Graphics.h"
#include "Input.h"
#include "PlayingState.h"
#include "ControlManager.h"
#include "TextMenu.h"
#include "Label.h"
#include "Client.h"
#include "LobbyState.h"
#include "Database.h"

BrowsingState BrowsingState::mBrowsingState;

void BrowsingState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mDatabase = new Database();
	mControlManager = new ControlManager("ui_layout.lua");

	BuildUi();

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/menu_bkgd.png");
}

void BrowsingState::Cleanup(void)
{

}

void BrowsingState::Pause()
{

}

void BrowsingState::Resume()
{

}

void BrowsingState::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

void BrowsingState::Update(GLib::Input* pInput, double dt)
{
	mControlManager->Update(pInput, dt);
}

void BrowsingState::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, GLib::GetClientWidth()/2, GLib::GetClientHeight()/2, GLib::GetClientWidth(), GLib::GetClientHeight());
	mControlManager->Draw(pGraphics);
}

void BrowsingState::BuildUi()
{
	vector<ServerData> serverList = mDatabase->GetServers();

	Label* title = new Label(800, 50, "StateHeader", "Select a server");
	mControlManager->AddControl(title);

	TextMenu* serverMenu = new TextMenu(800, 400, "ServerMenu");
	serverMenu->AddItemPressedListener(&BrowsingState::OnServerPressed, this);

	for(int i = 0; i < serverList.size(); i++)
	{
		// Add to server map.
		mServerMap[serverList[i].host] = serverList[i];

		char numPlayers[10];
		sprintf(numPlayers, "%i/4", serverList[i].numPlayers);

		string serverItem;
		serverItem.resize(50, ' ');

		serverItem.replace(0, serverList[i].host.length(), serverList[i].host);
		serverItem.replace(20, serverList[i].name.length(), serverList[i].name);
		//serverItem.replace(30, serverList[i].publicIp.length(), serverList[i].publicIp);
		serverItem.replace(43, string(numPlayers).length(), numPlayers);

		serverMenu->AddItem(serverList[i].host, serverItem);
	}

	serverMenu->PerformLayout();
	mControlManager->AddControl(serverMenu);
	mControlManager->LoadLuaProperties();
}

void BrowsingState::OnServerPressed(Label* pLabel)
{
	string host = pLabel->GetName();
	ServerData server = mServerMap[host];

	ChangeState(LobbyState::Instance());
	LobbyState::Instance()->SetServerData(server);

	//PlayingState::Instance()->GetClient()->ConnectToServer(server.localIp);

	//ChangeState(PlayingState::Instance());

	////if(!PlayingState::Instance()->GetClient()->ConnectToServer(server.publicIp))
	//	PlayingState::Instance()->GetClient()->ConnectToServer(server.localIp);
}

void BrowsingState::OnResize(float width, float height)
{
	mControlManager->OnResize(width, height);
}