#include "stdafx.h"
#include "Engine.h"
#include "Startup.h"
#include "Actor.h"
#include "Camera.h"

InputManager* GInputManager = nullptr;
IRenderer* GRenderer = nullptr;
Camera* GCamera = nullptr;
Camera* GTest = nullptr;
Camera* GCurrentCamera = nullptr;

typedef void (*DLL_FUNCTION_ARG3)(void**, HINSTANCE, const wchar_t*);
typedef bool (*DLL_FUNCTION_ARG1)(void**);

Engine::Engine()
	: application_(nullptr),
	renderer_(nullptr),
	applicationModule_(nullptr),
	rendererModule_(nullptr),
	prevFrameCheckTick_(0),
	prevUpdateTick_(0),
	frameCount_(0),
	startup_(nullptr),
	actorHead_(nullptr),
	actorTail_(nullptr),
	cameraHead_(nullptr),
	cameraTail_(nullptr)
{
}

Engine::~Engine()
{
	Cleanup();
}

Engine* Engine::CreateEngine()
{
	return new Engine;
}

void Engine::DestroyEngine(Engine* engine)
{
	if (nullptr == engine)
	{
		return;
	}

	delete engine;
}

bool Engine::Initialize(HINSTANCE hInstance, const wchar_t* icon, IStartup* startup)
{
	if (nullptr == startup)
	{
		return false;
	}

	if (false == LoadApplication(hInstance, icon))
	{
		return false;
	}

	if (false == LoadRenderer())
	{
		return false;
	}

	GInputManager = new InputManager;
	if (nullptr == GInputManager)
	{
#ifdef _DEBUG
		__debugbreak();
#endif // _DEBUG

		return false;
	}
	if (false == GInputManager->Initialize())
	{
		return false;
	}

	// 朝五虞 持失
	GCamera = new Camera;
	GCamera->BeginPlay();
	GCamera->SetClearColor(Color(1.0f, 0.0f, 0.0f, 0.0f));
	GCamera->SetSize(float2(2560.f, 1440.f));
	GCamera->GetTransform().SetPosition({ -5.0f, 0.0f, 5.0f, 1.0f });
	GCamera->GetTransform().AddRotaionY(40);
	GCamera->GetTransform().AddRotaionX(120);

	/*GTest = new Camera;
	GTest->BeginPlay();
	GTest->SetClearColor(Color(1.0f, 1.0f, 0.0f, 0.0f));
	GTest->SetSize(float2(2560.f, 1440.f));

	GTest->GetTransform().SetRotation({ 0.0f, 0.0f, 0.0f });
	GTest->GetTransform().SetPosition({ 0.0f, 5.0f, 5.0f, 1.0f });
	GTest->GetTransform().AddRotaionX(40);
	GTest->GetTransform().AddRotaionY(40);
	GTest->GetTransform().AddRotationZ(-90);*/

	LinkToLinkedListFIFO(&cameraHead_, &cameraTail_, ((Actor*)GCamera)->Link());
	//LinkToLinkedListFIFO(&cameraHead_, &cameraTail_, ((Actor*)GTest)->Link());

	if (false == InitializeStartup(startup))
	{
		return false;
	}

	return true;
}

void Engine::Run()
{
	while (false == application_->ApplicationQuit()) {

		application_->WinPumpMessage();

		unsigned long long curTick = GetTickCount64();
		if (prevUpdateTick_ == 0)
		{
			prevUpdateTick_ = prevFrameCheckTick_ = curTick;
		}

		// update
		Update(curTick);

		// render
		Render();

		renderer_->BeginRender();

		GCamera->Merge(float2(0.0f, 0.0f), float2(1.0f, 1.0f));

		//GCamera->Merge(float2(-0.5f, 0.0f), float2(0.5f, 1.0f));

		//GTest->Merge(float2(0.5f, 0.0f), float2(0.5f, 1.0f));

		renderer_->EndRender();
	}

	if (nullptr != startup_)
	{
		startup_->End();
		startup_->Release();
		startup_ = nullptr;
	}
}

Actor* Engine::GetMainCamera()
{
	return GCamera;
}

void Engine::CreateActorInternal(Actor* actor)
{
	if (nullptr == actor)
	{
		return;
	}

	actor->BeginPlayInternal();

	LinkToLinkedList(&actorHead_, &actorTail_, actor->Link());
}

void Engine::Update(unsigned long long tick)
{
	unsigned long long deltaTick = tick - prevUpdateTick_;
	if (deltaTick < 16) {
		return;
	}
	else if (20 <= deltaTick) {
		deltaTick = 16;
	}

	GInputManager->Tick(deltaTick);

	prevUpdateTick_ = tick;

	GCamera->Update();

	LINK_ITEM* current = actorHead_;
	while (current)
	{
		Actor* actor = (Actor*)current->item_;
		current = current->next_;

		if (nullptr == actor)
		{
			continue;
		}

		actor->UpdateInternal();
	}
}

void Engine::Render()
{

	LINK_ITEM* currentCamera = cameraHead_;
	while (currentCamera)
	{
		GCurrentCamera = (Camera*)currentCamera->item_;
		currentCamera = currentCamera->next_;

		GCurrentCamera->Render();

		LINK_ITEM* current = actorHead_;
		while (current)
		{
			Actor* actor = (Actor*)current->item_;
			current = current->next_;

			if (nullptr == actor)
			{
				continue;
			}

			actor->RenderInternal();
		}

		GCurrentCamera = nullptr;
	}

	GCurrentCamera = nullptr;
}

bool Engine::LoadApplication(HINSTANCE hInstance, const wchar_t* icon)
{
	applicationModule_ = LoadLibrary(L"WindowsApplication_x64_Debug.dll");
	if (!applicationModule_) {
		__debugbreak();
		return false;
	}

	DLL_FUNCTION_ARG3 loadFunction = (DLL_FUNCTION_ARG3)GetProcAddress(applicationModule_, "CreateWindowsApplication");
	if (!loadFunction) {
		__debugbreak();
		return false;
	}

	loadFunction((void**)&application_, hInstance, icon);
	if (nullptr == application_) {
		__debugbreak();
		return false;
	}

	if (false == application_->InitializeWindow()) {
		return false;
	}
	return true;
}

bool Engine::LoadRenderer()
{
	rendererModule_ = LoadLibrary(L"RendererD3D11_x64_Debug.dll");
	if (!rendererModule_)
	{
		__debugbreak();
		return false;
	}

	DLL_FUNCTION_ARG1 loadFunction1 = (DLL_FUNCTION_ARG1)GetProcAddress(rendererModule_, "CreateRenderer");
	if (!loadFunction1)
	{
		__debugbreak();
		return false;
	}

	loadFunction1((void**)&renderer_);
	if (nullptr == renderer_)
	{
		__debugbreak();
		return false;
	}

	if (false == renderer_->Initialize(application_->GetMainWindowHandle(), 2560, 1440))
	{
		__debugbreak();
		return false;
	}

	GRenderer = renderer_;

	return true;
}

bool Engine::InitializeStartup(IStartup* startup)
{
	if (nullptr == startup)
	{
		return false;
	}
	startup_ = startup;
	startup_->AddRef();

	if (false == startup_->Start())
	{
		return false;
	}

	return true;
}

void Engine::Cleanup()
{
	while (actorHead_)
	{
		Actor* actor = (Actor*)actorHead_->item_;

		UnLinkFromLinkedList(&actorHead_, &actorTail_, actor->Link());

		delete actor;
	}

	if (nullptr != GCamera)
	{
		delete GCamera;
		GCamera = nullptr;
	}
	if (nullptr != GTest)
	{
		delete GTest;
		GTest = nullptr;
	}

	if (nullptr != startup_)
	{
		startup_->Release();
		startup_ = nullptr;
	}
	if (nullptr != GInputManager)
	{
		delete GInputManager;
		GInputManager = nullptr;
	}
	if (nullptr != renderer_)
	{
		renderer_->Release();
		renderer_ = nullptr;
	}
	if (nullptr != application_)
	{
		application_->Release();
		application_ = nullptr;
	}

	if (nullptr != rendererModule_)
	{
		FreeLibrary(rendererModule_);
		rendererModule_ = nullptr;
	}
	if (nullptr != applicationModule_)
	{
		FreeLibrary(applicationModule_);
		applicationModule_ = nullptr;
	}
}
