#pragma once

#include "d3dUtil.h"
#include <list>

namespace GLib {
	class Graphics;
	class Object3D;
	class World;
	class Input;
}

struct Target
{
	XMFLOAT3 pos;
	XMFLOAT3 dir;
};

class Arena 
{
public:
	Arena();
	~Arena();

	void Init();
	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
private:
	GLib::Object3D* mSelectedObject;
	GLib::World*	mWorld;
	std::list<Target> mTargetQueue;
};