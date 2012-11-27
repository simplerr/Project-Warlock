#pragma once

#include "d3dUtil.h"
#include <list>

namespace GLib {
	class Graphics;
	class Object3D;
	class World;
	class Input;
}

class Actor;
class Client;

class Arena 
{
public:
	Arena();
	~Arena();

	void Init();
	void Update(GLib::Input* pInput, Client* pPeer, float dt);
	void Draw(GLib::Graphics* pGraphics);
	GLib::World* GetWorld();
private:
	GLib::World*	mWorld;
	Actor*	mSelectedObject;
};