#pragma once

#include "Actor.h"

class Camera final
	: public Actor
{
public:
	Camera();

	~Camera();

	void BeginPlay() override;

	void Update() override;

	void Render() override;

	void SetSize(const float2& size);

	void SetConfig(float fov, float Near, float Far);

	void SetClearColor(const Color& clearColor);

	const float4x4& View() const;
	const float4x4& Projection() const;

	void Merge(const float2& Offset, const float2& Scale);

private:
	void CameraTransformUpdate();

	float2 size_;

	float fov_;
	float near_;
	float far_;

	Color clearColor_;

	float4x4 view_;
	float4x4 projection_;

	IRenderTarget* renderTarget_;

	IVertex* vertex_;

	IInputLayOut* layout_;

	IConstantBuffer* constantBuffer_;

	IRasterizer* rasterizer_;

	IMaterial* material_;
};