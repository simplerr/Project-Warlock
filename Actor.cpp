#include "Actor.h"
#include "d3dUtil.h"
#include "Graphics.h"

Actor::Actor(GLib::ModelImporter* pImporter, string filename)
	: AnimatedObject(pImporter, filename)
{
	SetSelected(false);
	SetVelocity(XMFLOAT3(0, 0, 0));
	SetFriction(0.99f);
	Init();
}

Actor::~Actor()
{

}

void Actor::Init()
{

}

void Actor::Update(float dt)
{
	// Update the animaton.
	AnimatedObject::Update(dt);

	// Simple physics.
	mVelocity = mVelocity * mFriction;
	SetPosition(GetPosition() + mVelocity); 

	// Move the object.
	// [NOTE] The position of the actor gets set in the Client as well,
	// but it's overwritten with the UPDATE_WORLD MSG!
	if(mTargetQueue.size() != 0)
	{
		SetRotation(XMFLOAT3(0, atan2f(mTargetQueue.front().dir.x, mTargetQueue.front().dir.z), 0));
		XMFLOAT3 newPos = GetPosition() + mTargetQueue.front().dir * GetMovementSpeed();
		SetPosition(XMFLOAT3(newPos.x, GetPosition().y, newPos.z));

		// Reached the target?
		XMFLOAT3 diff = mTargetQueue.front().pos - GetPosition();
		if(sqrt(diff.x * diff.x + diff.z * diff.z) < 1.0f) 
			mTargetQueue.pop_front();
	}

	// Walk or idle animation?
	if(mTargetQueue.size() != 0)
		SetAnimation(1);
 	else
 		SetAnimation(0);
}

void Actor::Draw(GLib::Graphics* pGraphics)
{
	AnimatedObject::Draw(pGraphics);

	// Only draw target queue if selected.
	if(mSelected)
	{
		for(auto iter = mTargetQueue.begin(); iter != mTargetQueue.end(); iter++) 
		{
			XNA::AxisAlignedBox box;
			box.Center = (*iter).pos;
			box.Extents = XMFLOAT3(2, 2, 2);
			pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::White), 0.6f);
		}
	}
}

void Actor::AddTarget(XMFLOAT3 targetPos, bool clear)
{
	// Clear the queue?
	if(clear)
		mTargetQueue.clear();

	XMFLOAT3 diff;
	Target target;

	// Use the current position if standing still or not adding to queue.
	if(mTargetQueue.size () == 0 || clear)
		diff = targetPos - GetPosition();
	else
		diff = targetPos - mTargetQueue.back().pos;

	XMStoreFloat3(&target.dir, XMVector3Normalize(XMLoadFloat3(&diff)));
	target.pos = targetPos;

	mTargetQueue.push_back(target);
}

void Actor::ClearTargetQueue()
{
	mTargetQueue.clear();
}

void Actor::SetSelected(bool selected)
{
	mSelected = selected;
}

bool Actor::IsSelected()
{
	return mSelected;
}

void Actor::SetVelocity(XMFLOAT3 velocity)
{
	mVelocity = velocity;
}

void Actor::SetFriction(float friction)
{
	mFriction = friction;
}

float Actor::IsKnockedBack()
{
	return sqrt(mVelocity.x * mVelocity.x + mVelocity.z * mVelocity.z) > 0.09f;
}