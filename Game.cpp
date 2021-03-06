#include <crtdbg.h> 
//#include "vld.h"
#include <assert.h>
#include <time.h>
#include "Game.h"
#include "Graphics.h"
#include "Input.h"
#include "World.h"
#include "Primitive.h"
#include "Camera.h"
#include "Effects.h"
#include "CameraFPS.h"
#include "CameraRTS.h"
#include "Client.h"
#include "ControlManager.h"
#include "Label.h"
#include <fstream>
#include "TextMenu.h"
#include "GameState.h"
#include "BrowsingState.h"
#include "D3DCore.h"
#include "MainMenuState.h"
#include "OptionsState.h"
#include "Sound.h"
#include "ServerCvars.h"
#include "C:\Users\Axel\Documents\Visual Studio 2010\Memory_and_Exception_Trace\Stackwalker.h"
#include "ShadowMap.h"

using namespace GLib;

#define _HAS_ITERATOR_DEBUGGING 0
//#define _DEBUG
//#define VLD_FORCE_ENABLE 1

float mx, my;

// Set global to NULL.
GLib::Runnable* GLib::GlobalApp = nullptr;
Sound*	gSound = nullptr;
ServerCvars* gCvars = nullptr;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	InitAllocCheck(ACOutput_XML);

	// Create a Game instance.
	Game game(hInstance, "Client", 1600, 900);
	GLib::GlobalApp = &game;

	// Init the app.
	game.Init();

	//game.SwitchScreenMode();

	float width = GetSystemMetrics(SM_CXSCREEN);
	float height = GetSystemMetrics(SM_CYSCREEN);

	//width = 1600;
	//height = 1080;

	game.ResizeWindow(width, height);

	// Run the app.
	return GLib::GlobalApp->Run();
}

Game::Game(HINSTANCE hInstance, string caption, int width, int height)
	: Runnable(hInstance, caption, width, height)
{
	// Cap the fps to 100.
	SetFpsCap(99.0f);
	mCurrentState = nullptr;

	gSound = new Sound();
	gCvars = new ServerCvars();
}
	
Game::~Game()
{
	mCurrentState->Cleanup();

	delete gSound;
	delete gCvars;

	DeInitAllocCheck();
}

void Game::Init()
{
	SetUseWindowBorder(false);

	// Important to run Systems Init() function.
	Runnable::Init();

	// Add a camera.
	GLib::CameraRTS* camera = new GLib::CameraRTS();
	GetGraphics()->SetCamera(camera);

	// Set the fog color.
	GetGraphics()->SetFogColor(XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));

	ChangeState(MainMenuState::Instance());

	mDrawDebug = false;
}

void Game::Update(GLib::Input* pInput, float dt)
{
	mCurrentState->Update(pInput, dt);

	GetGraphics()->Update(pInput, dt);

	mx = pInput->MousePosition().x;
	my = pInput->MousePosition().y;

	if(pInput->KeyPressed(VK_F1))
		mDrawDebug = !mDrawDebug;

	/*if(pInput->KeyReleased('F')) {
		float width = GetSystemMetrics(SM_CXSCREEN);
		float height = GetSystemMetrics(SM_CYSCREEN);

		ResizeWindow(width, height);
	}*/
}
	
void Game::Draw(GLib::Graphics* pGraphics)
{
	// Clear the render target and depth/stencil.
	pGraphics->ClearScene();

	mCurrentState->Draw(pGraphics);
	pGraphics->DrawBillboards();

	if(mDrawDebug)
	{
		char buffer[244];
		sprintf(buffer, "x: %.2f\ny: %.2f\nFPS:%.2f", mx, my, GetCurrentFps());
		pGraphics->DrawText(buffer, GLib::GetClientWidth()-100, 400, 20, GLib::ColorRGBA(255, 255, 255, 255));
	}

	// Present the backbuffer.
	pGraphics->Present();

	// Unbind the SRVs from the pipeline so they can be used as DSVs instead.
	ID3D11ShaderResourceView *const nullSRV[4] = {NULL, NULL, NULL, NULL};
	pGraphics->GetContext()->PSSetShaderResources(0, 4, nullSRV);
	Effects::BasicFX->Apply(GetD3DContext());
}

void Game::ChangeState(GameState* pGameState)
{
	// Cleanup the old state.
	if(mCurrentState != NULL)
		mCurrentState->Cleanup();

	// Set and init the new state.
	mCurrentState = pGameState;
	mCurrentState->Init(this);
}

//! Called when the window gets resized.
void Game::OnResize(int width, int height)
{
	mCurrentState->OnResize(width, height);
}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(mCurrentState != nullptr)
		mCurrentState->MsgProc(msg, wParam, lParam);

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}