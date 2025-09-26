#include "stdafx.h"
#include "TestActor.h"

TestActor::TestActor()
	:camera_(nullptr)
{
}

TestActor::~TestActor()
{
}

void TestActor::BeginPlay()
{
	Engine* engine = EngineContainer->GetEngine();

	camera_ = engine->GetMainCamera();
}

void TestActor::Update()
{
	if (false == InputManager::Instance()->IsAnyKeyPress())
	{
		return;
	}

	if (InputManager::Instance()->IsPress('w'))
	{
		camera_->GetTransform().AddPosition(camera_->GetTransform().ForwardVector());
	}
	if (InputManager::Instance()->IsPress('s'))
	{
		float4 back = camera_->GetTransform().ForwardVector();
		back.X *= -1;
		back.Y *= -1;
		back.Z *= -1;
		camera_->GetTransform().AddPosition(back);
	}
	if (InputManager::Instance()->IsPress('a'))
	{
		float4 left = camera_->GetTransform().RightVector();
		left.X *= -1;
		left.Y *= -1;
		left.Z *= -1;
		camera_->GetTransform().AddPosition(left);
	}
	if (InputManager::Instance()->IsPress('d'))
	{
		camera_->GetTransform().AddPosition(camera_->GetTransform().RightVector());
	}

	if (InputManager::Instance()->IsPress('e'))
	{
		camera_->GetTransform().AddRotationZ(1);
	}
	if (InputManager::Instance()->IsPress('q'))
	{
		camera_->GetTransform().AddRotationZ(-1);
	}

}
