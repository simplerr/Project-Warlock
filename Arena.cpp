#include "Arena.h"
#include "World.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "ModelImporter.h"
#include "StaticObject.h"
#include "Input.h"
#include "Light.h"
#include "LivingObject.h"
#include "Peer.h"

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

	LivingObject* object = new LivingObject(GLib::GetGraphics()->GetModelImporter(), "models/monster/monster.x");
	object->SetPosition(XMFLOAT3(0, 4, 0));
	object->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	mWorld->AddObject(object);

	// Connect the graphics light list.
	GLib::GetGraphics()->SetLightList(mWorld->GetLights());
}

void Arena::Update(GLib::Input* pInput, Peer* pPeer, float dt)
{
	// Update the world.
	mWorld->Update(dt);

	// Get the selected object.
	if(pInput->KeyPressed(VK_LBUTTON))
	{
		LivingObject* selected = (LivingObject*)mWorld->GetSelectedObject(pInput->GetWorldPickingRay());
		if(selected != nullptr) 
		{
			if(mSelectedObject != nullptr) {
				mSelectedObject->SetSelected(false);
				mSelectedObject->SetMaterials(GLib::Material(GLib::Colors::White));
			}

			mSelectedObject = selected;
			mSelectedObject->SetSelected(true);
			mSelectedObject->SetMaterials(GLib::Material(XMFLOAT4(1.0f, 127.0f/255.0f, 38/255.0f, 0.12f) * 4));
		}
	}

	// Add movement target for the selected object.
	if(pInput->KeyPressed(VK_RBUTTON) && mSelectedObject != nullptr)
	{
		XMFLOAT3 pos = mWorld->GetTerrainIntersectPoint(pInput->GetWorldPickingRay());

		// Inform the server about what happened.
		// The server then informs all the clients, including the callee.
		if(pos.x != numeric_limits<float>::infinity())
			pPeer->SendAddTarget(mSelectedObject->GetId(), pos, pInput->KeyDown(VK_SHIFT) ? false : true);
	}
}

void Arena::Draw(GLib::Graphics* pGraphics)
{
	// Draw the world.
	mWorld->Draw(pGraphics);
}

GLib::World* Arena::GetWorld()
{
	return mWorld;
}