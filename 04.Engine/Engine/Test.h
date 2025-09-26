#pragma once

struct SimpleVertex {
	float3 position;
	Color color;
	float3 normal;
	float2 UV;
	float4 tangent;

	SimpleVertex(float3 pos, Color tmpcolor, float3 tmpnormal, float2 uv)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent() {
	}
};

struct ConstantBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
	float4 color;
	float4 lightDir;
	float4 lightColor;
	float4 ambientColor;

	float3 spotPosition;
	float spotRange;
	float3 spotDirection;
	float spotAngle;

	// point light
	float3 pointPosition;
	float pointLightRange;

	// °¨¼è °è¼ö
	float attenuationConst;
	float attenuationLinear;
	float attenuationQuad;
	float pad1;
};

struct MergeVertex
{
	float2 Pos;
	float2 UV;
};

struct MergeConstantBuffer
{
	float2 Offset;
	float2 Scale;
};

bool CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius = 0.5f);

bool CreateRect(std::vector<MergeVertex>* outVertices, std::vector<WORD>* outIndices);