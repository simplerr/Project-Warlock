#include <crtdbg.h> 
#include <assert.h>
#include <time.h>
#include "Game.h"
#include "Graphics.h"
#include "Input.h"
#include "World.h"
#include "vld.h"
#include "Primitive.h"
#include "Camera.h"
#include "Effects.h"
#include "CameraFPS.h"
#include "CameraRTS.h"

using namespace GLib;

// Set global to NULL.
GLib::Runnable* GLib::GlobalApp = nullptr;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Create a Game instance.
	Game game(hInstance, "Project Warlock", 1200, 800);
	GLib::GlobalApp = &game;

	// Init the app.
	game.Init();

	// Run the app.
	return GLib::GlobalApp->Run();
}

Game::Game(HINSTANCE hInstance, string caption, int width, int height)
	: Runnable(hInstance, caption, width, height)
{
	// Cap the fps to 100.
	//SetFpsCap(100.0f);
}
	
Game::~Game()
{
	delete mWorld;
}

void Game::Init()
{
	// Important to run Systems Init() function.
	Runnable::Init();

	// Add a camera.
	GLib::CameraRTS* camera = new GLib::CameraRTS();
	GetGraphics()->SetCamera(camera);

	// Create the world.
	mWorld = new GLib::World();
	mWorld->Init(GetGraphics());

	// Connect the graphics light list and set the fog color.
	GetGraphics()->SetLightList(mWorld->GetLights());
	GetGraphics()->SetFogColor(XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
}

void Game::Update(GLib::Input* pInput, float dt)
{
	mWorld->Update(dt);
}
	
void Game::Draw(GLib::Graphics* pGraphics)
{
	// Clear the render target and depth/stencil.
	pGraphics->ClearScene();

	// Draw all objects.
	mWorld->Draw(pGraphics);	
	pGraphics->DrawBillboards();

	// Present the backbuffer.
	pGraphics->Present();

	// Unbind the SRVs from the pipeline so they can be used as DSVs instead.
	ID3D11ShaderResourceView *const nullSRV[4] = {NULL, NULL, NULL, NULL};
	pGraphics->GetContext()->PSSetShaderResources(0, 4, nullSRV);
	Effects::BasicFX->Apply(GetD3DContext());
}

//! Called when the window gets resized.
void Game::OnResize(int width, int height)
{

}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Toggle screen mode?
	if(msg == WM_CHAR) {
		if(wParam == 'f')	
			SwitchScreenMode();
	}

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}