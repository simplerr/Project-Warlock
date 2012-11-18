#include "Arena.h"
#include "World.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "ModelImporter.h"
#include "StaticObject.h"
#include "Input.h"
#include "Light.h"

Arena::Arena()
{
	mSelectedObject = nullptr;
}

Arena::~Arena()
{
	delete mWorld;
}

void Arena::Init()
{
	// Create the world.
	mWorld = new GLib::World();
	mWorld->Init(GLib::GetGraphics());

	GLib::StaticObject* box = new GLib::StaticObject(GLib::GetGraphics()->GetModelImporter(), "models/misc/crate.obj");
	box->SetPosition(XMFLOAT3(0, 2, 0));
	box->SetScale(XMFLOAT3(4, 4, 4));
	mWorld->AddObject(box);

	// Connect the graphics light list.
	GLib::GetGraphics()->SetLightList(mWorld->GetLights());
}

void Arena::Update(GLib::Input* pInput, float dt)
{
	// Update the world.
	mWorld->Update(dt);

	// Get the selected object.
	if(pInput->KeyPressed(VK_LBUTTON))
	{
		GLib::Object3D* selected = mWorld->GetSelectedObject(pInput->GetWorldPickingRay());
		if(selected != nullptr) 
		{
			mSelectedObject = selected;
			mSelectedObject->SetMaterials(GLib::Material(XMFLOAT4(1.0f, 127.0f/255.0f, 38/255.0f, 0.12f) * 4));
		}
	}

	// Add movement target for the selected object.
	if(pInput->KeyPressed(VK_RBUTTON) && mSelectedObject != nullptr)
	{
		Target target;
		target.pos = mWorld->GetTerrainIntersectPoint(pInput->GetWorldPickingRay());

		// Inside the terrain?
		if(target.pos.x != numeric_limits<float>::infinity())
		{
			XMFLOAT3 diff;
			
			// Use the current position if standing still or not adding to queue.
			if(mTargetQueue.size () == 0 || !pInput->KeyDown(VK_SHIFT))
				diff = target.pos - mSelectedObject->GetPosition();
			else
				diff = target.pos - mTargetQueue.back().pos;

			XMStoreFloat3(&target.dir, XMVector3Normalize(XMLoadFloat3(&diff)));

			// Clear the target queue if SHIFT is not held down.
			if(!pInput->KeyDown(VK_SHIFT))
				mTargetQueue.clear();

			// Add to queue end.
			mTargetQueue.push_back(target);
		}
	}

	// Move the object.
	if(mTargetQueue.size() != 0)
	{
		float speed = 0.10f;
		mSelectedObject->SetPosition(mSelectedObject->GetPosition() + mTargetQueue.front().dir * speed);

		// Reached the target?
		XMFLOAT3 diff = mTargetQueue.front().pos - mSelectedObject->GetPosition();
		if(sqrt(diff.x * diff.x + diff.z * diff.z) < 1.0f) {
			mTargetQueue.pop_front();

			if(mTargetQueue.size() == 0)
				int a = 1;
		}
	}
}

void Arena::Draw(GLib::Graphics* pGraphics)
{
	// Draw the world.
	mWorld->Draw(pGraphics);

	for(auto iter = mTargetQueue.begin(); iter != mTargetQueue.end(); iter++) {
		XNA::AxisAlignedBox box;
		box.Center = (*iter).pos;
		box.Extents = XMFLOAT3(2, 2, 2);
		pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::White), 0.6f);
	}
}