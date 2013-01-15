#include "BaseArena.h"
#include "World.h"
#include "Graphics.h"

BaseArena::BaseArena()
{
	// Create the world.
	mWorld = new GLib::World();
	mWorld->Init(GLib::GetGraphics());

	// Connect the graphics light list.
	GLib::GetGraphics()->SetLightList(mWorld->GetLights());
}

BaseArena::~BaseArena()
{
	delete mWorld;
}