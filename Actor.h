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
	void SetMovementSpeed(float movementSpeed);
	void SetSelected(bool selected);
	void SetVelocity(XMFLOAT3 velocity);
	void SetFriction(float friction);
	bool IsSelected();

	float GetMovementSpeed();
	float IsKnockedBack();
private:
	std::list<Target> mTargetQueue;
	float			  mMovementSpeed;
	bool			  mSelected;

	float mFriction;
	XMFLOAT3 mVelocity;
};