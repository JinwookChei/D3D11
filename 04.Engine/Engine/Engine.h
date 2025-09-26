#pragma once

class Actor;
struct IStartup;
class Engine final {
public:
	Engine();

	~Engine();

	ENGINE_API static Engine* CreateEngine();

	ENGINE_API static void DestroyEngine(Engine* engine);

	ENGINE_API bool Initialize(HINSTANCE hInstance, const wchar_t* icon, IStartup* startup);

	ENGINE_API void Run();

	ENGINE_API Actor* GetMainCamera();

	template <typename ActorType>
	ActorType* CreateActor()
	{
		ActorType* newActor = new ActorType;

		CreateActorInternal(newActor);

		return newActor;
	}

private:
	ENGINE_API void CreateActorInternal(Actor* actor);

	void Update(unsigned long long tick);

	void Render();

	bool LoadApplication(HINSTANCE hInstance, const wchar_t* icon);

	bool LoadRenderer();

	bool InitializeStartup(IStartup* startup);

	void Cleanup();

	unsigned long long prevFrameCheckTick_;
	unsigned long long prevUpdateTick_;
	unsigned int frameCount_;

	IStartup* startup_;

	IApplication* application_;

	IRenderer* renderer_;

	HMODULE applicationModule_;

	HMODULE rendererModule_;

	LINK_ITEM* actorHead_;
	LINK_ITEM* actorTail_;

	LINK_ITEM* cameraHead_;
	LINK_ITEM* cameraTail_;
};