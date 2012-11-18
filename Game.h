#pragma once
#include "Runnable.h"

// Forward declarations.
#pragma region Forward declarations.
namespace GLib {
	class Light;
	class RenderTarget;
	class Primitive;
	class Texture2D;
	class ShadowMap;
	class ModelImporter;
	class Model;
	class SkinnedModel;
	class World;
	class StaticObject;
	class AnimatedObject;
}
#pragma endregion

class Game : public GLib::Runnable
{
public:
	Game(HINSTANCE hInstance, string caption, int width, int height);
	~Game();

	void Init();
	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	void OnResize(int width, int height);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	GLib::World*			mWorld;
	GLib::StaticObject*		mTerrain;
	GLib::AnimatedObject*	mAnimatedObject;
	GLib::Texture2D*		mLoadingAssetsBkgd;
	GLib::Texture2D*		mHelpScreen;
	GLib::Texture2D*		mPressH;
};