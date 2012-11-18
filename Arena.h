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

class Arena 
{
public:
	Arena();
	~Arena();

	void Init();
	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
private:
	GLib::World*	mWorld;
	LivingObject*	mSelectedObject;
};