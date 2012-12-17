#include "BrowsingState.h"
#include "Graphics.h"
#include "Input.h"
#include "PlayingState.h"
#include "ControlManager.h"
#include "TextMenu.h"
#include "Label.h"
#include "Client.h"

BrowsingState BrowsingState::mBrowsingState;

void BrowsingState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mDatabase = new Database();
	mControlManager = new ControlManager("ui_layout.lua");

	BuildUi();
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

	if(pInput->KeyPressed(VK_SPACE))
		ChangeState(PlayingState::Instance());
}

void BrowsingState::Draw(GLib::Graphics* pGraphics)
{
	mControlManager->Draw(pGraphics);

	pGraphics->DrawText("BrowsingState", 600, 400, 60);
}

void BrowsingState::BuildUi()
{
	vector<ServerData> serverList = mDatabase->GetServers();

	TextMenu* serverMenu = new TextMenu(600, 400, "ServerMenu");
	serverMenu->AddItemPressedListener(&BrowsingState::OnServerPressed, this);

	for(int i = 0; i < serverList.size(); i++)
	{
		// Add to server map.
		mServerMap[serverList[i].host] = serverList[i];

		char numPlayers[10];
		sprintf(numPlayers, "%i/4", serverList[i].numPlayers);

		string serverItem;
		serverItem.resize(62, ' ');

		serverItem.replace(0, serverList[i].host.length(), serverList[i].host);
		serverItem.replace(20, serverList[i].publicIp.length(), serverList[i].publicIp);
		serverItem.replace(40, string(numPlayers).length(), numPlayers);

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

	ChangeState(PlayingState::Instance());

	//if(!PlayingState::Instance()->GetClient()->ConnectToServer(server.publicIp))
		PlayingState::Instance()->GetClient()->ConnectToServer(server.localIp);
}