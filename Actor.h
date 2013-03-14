#pragma once

#include "AnimatedObject.h"
#include <string>
#include <list>

namespace GLib {
	class ModelImporter;
	class Graphics;
}

struct Target
{
	XMFLOAT3 pos;
	XMFLOAT3 dir;
};


//
//	Contains a target queue.
//
class Actor : public GLib::AnimatedObject
{
public:
	Actor(GLib::ModelImporter* pImporter, string filename);
	virtual ~Actor();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(GLib::Graphics* pGraphics);

	void AddTarget(XMFLOAT3 targetPos, bool clear);
	void ClearTargetQueue();
	virtual void SetMovementSpeed(float movementSpeed) = 0;
	void SetSelected(bool selected);
	void SetVelocity(XMFLOAT3 velocity);
	void SetFriction(float friction);
	void SetSlow(float slow);
	bool IsSelected();

	virtual float GetMovementSpeed() = 0;
	float IsKnockedBack();
private:
	std::list<Target> mTargetQueue;
	bool			  mSelected;

	float	 mFriction;
	float	 mSlow;
	XMFLOAT3 mVelocity;
};