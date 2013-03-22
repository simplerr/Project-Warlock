#include "Actor.h"
#include "d3dUtil.h"
#include "Graphics.h"

Actor::Actor(GLib::ModelImporter* pImporter, string filename)
	: AnimatedObject(pImporter, filename)
{
	SetSelected(false);
	SetVelocity(XMFLOAT3(0, 0, 0));
	SetFriction(0.99f);
	SetSlow(0.0f);
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

	/*if(mSlow < 0.2f)
		AdjustAnimationSpeedBy(110);
	if(mSlow >= 0.2f)
		AdjustAnimationSpeedBy(100);
	else if(mSlow >= 60)
		AdjustAnimationSpeedBy(90);*/

	// Move the object.
	// [NOTE] The position of the actor gets set in the Client as well,
	// but it's overwritten with the UPDATE_WORLD MSG!
	if(mTargetQueue.size() != 0)
	{
		// Only rotate when walking.
		if(GetCurrentAnimation() == 0)
			SetRotation(XMFLOAT3(0, atan2f(-mTargetQueue.front().dir.x, -mTargetQueue.front().dir.z), 0));

		XMFLOAT3 newPos = GetPosition() + mTargetQueue.front().dir * GetMovementSpeed() * (1 - mSlow);
		SetPosition(XMFLOAT3(newPos.x, GetPosition().y, newPos.z));

		// Reached the target?
		XMFLOAT3 diff = mTargetQueue.front().pos - GetPosition();
		if(sqrt(diff.x * diff.x + diff.z * diff.z) < 1.0f) 
			mTargetQueue.pop_front();
	}

	// Walk or idle animation?
	if(GetCurrentAnimation() == 0 || GetCurrentAnimation() == 1)
	{
		if(mTargetQueue.size() != 0)
			SetAnimation(0);
		else
			SetAnimation(1);
	}
}

void Actor::Draw(GLib::Graphics* pGraphics)
{
	AnimatedObject::Draw(pGraphics);

	// Only draw target queue if selected.
	/*if(mSelected)
	{
		for(auto iter = mTargetQueue.begin(); iter != mTargetQueue.end(); iter++) 
		{
			XNA::AxisAlignedBox box;
			box.Center = (*iter).pos;
			box.Extents = XMFLOAT3(2, 2, 2);
			pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::White), 0.6f);
		}
	}*/
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

	mVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	mTargetQueue.push_back(target);
}

void Actor::ClearTargetQueue()
{
	mTargetQueue.clear();
	SetAnimation(1);
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
	return sqrt(mVelocity.x * mVelocity.x + mVelocity.z * mVelocity.z) > 0.2f;
}

void Actor::SetSlow(float slow)
{
	mSlow = slow;
}