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

class LivingObject : public GLib::AnimatedObject
{
public:
	LivingObject(GLib::ModelImporter* pImporter, string filename);
	virtual ~LivingObject();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(GLib::Graphics* pGraphics);

	void AddTarget(XMFLOAT3 targetPos, bool clear);
	void SetMovementSpeed(float movementSpeed);
	void SetSelected(bool selected);
private:
	std::list<Target> mTargetQueue;
	float			  mMovementSpeed;
	bool			  mSelected;
};