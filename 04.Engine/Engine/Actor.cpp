#include "stdafx.h"
#include "Actor.h"
#include "Renderer.h"

Actor::Actor()
	: renderer_(new Renderer(this)),
	transform_(new Transform) {
	link_.prev_ = nullptr;
	link_.next_ = nullptr;
	link_.item_ = this;
}

Actor::~Actor()
{
	if (nullptr != renderer_)
	{
		delete renderer_;
		renderer_ = nullptr;
	}

	if (nullptr != transform_)
	{
		delete transform_;
		transform_ = nullptr;
	}
}

void Actor::BeginPlay()
{
}

void Actor::Update()
{
}

void Actor::Render()
{
}

Transform& Actor::GetTransform()
{
	return *transform_;
}

void Actor::BeginPlayInternal()
{
	renderer_->Create();

	BeginPlay();
}

void Actor::UpdateInternal()
{
	Update();
}

void Actor::RenderInternal()
{
	Render();

	if (nullptr == renderer_)
	{
		return;
	}

	renderer_->Render();
}

LINK_ITEM* Actor::Link()
{
	return &link_;
}
