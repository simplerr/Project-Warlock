#include "LivingObject.h"
#include "d3dUtil.h"
#include "Graphics.h"

LivingObject::LivingObject(GLib::ModelImporter* pImporter, string filename)
	: AnimatedObject(pImporter, filename)
{
	SetMovementSpeed(0.03f);
	SetSelected(false);
}

LivingObject::~LivingObject()
{

}

void LivingObject::Init()
{

}

void LivingObject::Update(float dt)
{
	AnimatedObject::Update(dt);

	// Move the object.
	if(mTargetQueue.size() != 0)
	{
		SetPosition(GetPosition() + mTargetQueue.front().dir * mMovementSpeed);

		// Reached the target?
		XMFLOAT3 diff = mTargetQueue.front().pos - GetPosition();
		if(sqrt(diff.x * diff.x + diff.z * diff.z) < 1.0f) {
			mTargetQueue.pop_front();

			if(mTargetQueue.size() == 0)
				int a = 1;
		}
	}

	// Walk or idle animation?
	if(mTargetQueue.size() != 0)
		SetAnimation(1);
 	else
 		SetAnimation(0);
}

void LivingObject::Draw(GLib::Graphics* pGraphics)
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

void LivingObject::AddTarget(XMFLOAT3 targetPos, bool clear)
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

void LivingObject::SetMovementSpeed(float movementSpeed)
{
	mMovementSpeed = movementSpeed;
}

void LivingObject::SetSelected(bool selected)
{
	mSelected = selected;
}