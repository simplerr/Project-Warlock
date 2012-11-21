#pragma once

#include "d3dUtil.h"
#include <list>

namespace GLib {
	class Graphics;
	class Object3D;
	class World;
	class Input;
}

class LivingObject;
class Peer;

class Arena 
{
public:
	Arena();
	~Arena();

	void Init();
	void Update(GLib::Input* pInput, Peer* pPeer, float dt);
	void Draw(GLib::Graphics* pGraphics);
	GLib::World* GetWorld();
private:
	GLib::World*	mWorld;
	LivingObject*	mSelectedObject;
};