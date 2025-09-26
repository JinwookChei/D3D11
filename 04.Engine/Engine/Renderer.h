#pragma once

#include "Transform.h"

class Actor;
class Renderer {
	friend class Actor;

public:
	explicit Renderer(Actor* actor);

	~Renderer();

	void Create();

	void Render();

private:
	Actor* owner_;

	IVertex* vertex_;

	IInputLayOut* layout_;

	IConstantBuffer* constantBuffer_;

	IRasterizer* rasterizer_;

	IMaterial* material_;
};