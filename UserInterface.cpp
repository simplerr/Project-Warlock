#include "UserInterface.h"
#include "ItemLoaderXML.h"
#include "Inventory.h"
#include "Shop.h"
#include "Client.h"
#include "Player.h"
#include "Graphics.h"

UserInterface::UserInterface(Client* pClient)
{
	mItemLoader = new ItemLoaderXML("items.xml");

	mInventory = new Inventory(900, 770, 3, 60);
	mInventory->SetItemLoader(mItemLoader);
	mInventory->SetClient(pClient);

	mShop = new Shop(200, 770, 3, 60);
	mShop->SetClient(pClient);
	mShop->SetItemLoader(mItemLoader);
	mShop->SetInspectingInventory(mInventory);

	mShop->PlaceInFreeSlot(ItemKey(REGEN_CAP, 1));
	mShop->PlaceInFreeSlot(ItemKey(IRON_ARMOR, 1));

	mBkgdTexture = GLib::GetGraphics()->LoadTexture("textures/ui_bkgd.png");
}

UserInterface::~UserInterface()
{
	delete mShop;
	delete mInventory;
	delete mItemLoader;
}

void UserInterface::Update(GLib::Input* pInput, float dt)
{
	mInventory->Update(pInput, dt);
	mShop->Update(pInput, dt);
}

void UserInterface::Draw(GLib::Graphics* pGraphics)
{
	pGraphics->DrawScreenQuad(mBkgdTexture, 600, 800, 1200, 200);
	mInventory->Draw(pGraphics);
	mShop->Draw(pGraphics);
}

void UserInterface::HandleItemAdded(Player* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);
	
	pPlayer->AddItem(mItemLoader, ItemKey(name, level));
	mInventory->UpdateItems();
}

void UserInterface::HandleItemRemoved(Player* pPlayer, RakNet::BitStream& bitstream)
{
	ItemName name;
	int playerId, level;
	bitstream.Read(name);
	bitstream.Read(level);

	pPlayer->RemoveItem(mItemLoader, ItemKey(name, level));
	mInventory->UpdateItems();
}

void UserInterface::SetSelectedPlayer(Player* pPlayer)
{
	mInventory->SetPlayer(pPlayer);
}

bool UserInterface::PointInsideUi(XMFLOAT3 position)
{
	return (position.y > GLib::GetClientHeight() - 200);
}