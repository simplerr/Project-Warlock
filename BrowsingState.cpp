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
#include "Button.h"
#include "MainMenuState.h"

BrowsingState BrowsingState::mBrowsingState;

void BrowsingState::Init(Game* pGame)
{
	// Important!!
	SetGame(pGame);

	mDatabase = new Database();
	mControlManager = new ControlManager("data/lua/ui_layout.lua");

	BuildUi();

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("data/textures/menu_bkgd.png");

	mSelectedServerHost = "none";
	mBackToMenu = false;
}

void BrowsingState::Cleanup(void)
{
	delete mDatabase;
	delete mControlManager;
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

	if(mSelectedServerHost != "none")
	{
		ServerData server = mServerMap[mSelectedServerHost];

		ChangeState(LobbyState::Instance());
		LobbyState::Instance()->SetServerData(server);
	}

	if(mBackToMenu)
		ChangeState(MainMenuState::Instance());
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

	mServerMenu = new TextMenu(800, 400, "ServerMenu");
	mServerMenu->AddItemPressedListener(&BrowsingState::OnServerPressed, this);
	mServerMenu->SetPressedSound("data/sounds/button.wav");

	RefreshServerList();

	Button* backButton = new Button(0, 0, "BrowsingBackButton", "Back");
	Button* refreshButton = new Button(0, 0, "BrowsingRefreshButton", "Refresh");
	refreshButton->AddPressedListener(&BrowsingState::OnButtonPressed, this);
	backButton->AddPressedListener(&BrowsingState::OnButtonPressed, this);

	mServerMenu->PerformLayout();
	mControlManager->AddControl(mServerMenu);
	mControlManager->AddControl(backButton);
	mControlManager->AddControl(refreshButton);
	mControlManager->LoadLuaProperties();
}

void BrowsingState::OnServerPressed(Label* pLabel)
{
	if(pLabel->GetName() != "no_servers_found")
		mSelectedServerHost = pLabel->GetName();
}

void BrowsingState::OnResize(float width, float height)
{
	mControlManager->OnResize(width, height);
}

void BrowsingState::OnButtonPressed(Button* pButton)
{
	if(pButton->GetName() == "BrowsingRefreshButton")
		RefreshServerList();
	else if(pButton->GetName() == "BrowsingBackButton")
		mBackToMenu = true;
}

void BrowsingState::RefreshServerList()
{
	mServerMenu->ClearItems();

	vector<ServerData> serverList = mDatabase->GetServers();

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

		mServerMenu->AddItem(serverList[i].host, serverItem);
	}

	if(serverList.size() == 0)
		mServerMenu->AddItem("no_servers_found", "No servers available");

	mServerMenu->PerformLayout();
}