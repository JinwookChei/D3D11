#pragma once

#include "Transform.h"

class Renderer;
class Actor {
	friend class Engine;

public:
	ENGINE_API Actor();

	ENGINE_API virtual ~Actor();

	ENGINE_API virtual void BeginPlay();

	ENGINE_API virtual void Update();

	ENGINE_API virtual void Render();

	ENGINE_API Transform& GetTransform();

private:
	void BeginPlayInternal();

	void UpdateInternal();

	void RenderInternal();

	LINK_ITEM* Link();

	LINK_ITEM link_;

	Transform* transform_;

	Renderer* renderer_;

};