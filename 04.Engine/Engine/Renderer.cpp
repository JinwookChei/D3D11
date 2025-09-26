#include "stdafx.h"
#include "Renderer.h"
#include "Test.h"
#include "Actor.h"
#include "Camera.h"

Renderer::Renderer(Actor* actor)
	: vertex_(nullptr),
	layout_(nullptr),
	constantBuffer_(nullptr),
	rasterizer_(nullptr),
	material_(nullptr),
	owner_(actor)
{
}

Renderer::~Renderer()
{
	if (nullptr != vertex_)
	{
		vertex_->Release();
		vertex_ = nullptr;
	}
	if (nullptr != layout_)
	{
		layout_->Release();
		layout_ = nullptr;
	}
	if (nullptr != constantBuffer_)
	{
		constantBuffer_->Release();
		constantBuffer_ = nullptr;
	}
	if (nullptr != rasterizer_)
	{
		rasterizer_->Release();
		rasterizer_ = nullptr;
	}
	if (nullptr != material_)
	{
		material_->Release();
		material_ = nullptr;
	}
}

void Renderer::Create()
{
	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> index;
	CreateSphere(&sphereVertices, &index);

	vertex_ = GRenderer->CreateVertex(sphereVertices.data(), (UINT)sizeof(SimpleVertex), (UINT)sphereVertices.size(), index.data(), (UINT)sizeof(WORD), (UINT)index.size());

	vertex_->AddInputLayOut("POSITION", 6, false, 0, 0);
	vertex_->AddInputLayOut("COLOR", 2, false, 0, 0);
	vertex_->AddInputLayOut("NORMAL", 6, false, 0, 0);
	vertex_->AddInputLayOut("TEXCOORD", 16, false, 0, 0);
	vertex_->AddInputLayOut("TANGENT", 2, false, 0, 0);

	IShader* vertexShader = GRenderer->CreateShader(ShaderType::Vertex, L"VertexShader.cso");
	IShader* pixelShader = GRenderer->CreateShader(ShaderType::Pixel, L"PixelShader.cso");
	ISamplerState* sampler = GRenderer->CreateSampler(true, false);

	layout_ = GRenderer->CreateLayOut(vertex_, vertexShader);

	material_ = GRenderer->CreateMaterial();

	constantBuffer_ = GRenderer->CreateConstantBuffer((unsigned int)sizeof(ConstantBuffer));

	rasterizer_ = GRenderer->CreateRasterizer(true);

	material_->SetVertexShader(vertexShader);
	material_->SetPixelShader(pixelShader);
	material_->SetSampler(sampler, 0);

	vertexShader->Release();
	pixelShader->Release();
	sampler->Release();

	owner_->GetTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

// ���Ǿ� ����
// ī�޶� ����
// ����Ʈ ����

void Renderer::Render()
{
	// ���Ǿ�
	//DirectX::XMMATRIX parentPosition = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	//DirectX::XMMATRIX parent = DirectX::XMMatrixRotationZ(1.0f) * parentPosition; // ������Ʈ ������ ��

	// ī�޶�
	//DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(-5.0f, 0.0f, 5.0f, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	//DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);

	// ��� ���� ������Ʈ
	ConstantBuffer cb;
	cb.world = owner_->GetTransform().GetWorldMatrixTranspose(); // ���Ǿ�
	MatrixTranspose(cb.view, GCurrentCamera->View()); // ī�޶�
	MatrixTranspose(cb.projection, GCurrentCamera->Projection()); // ī�޶�
	cb.color = float4(1.0f, 0.0f, 0.0f, 1.0f);

	cb.lightDir = float4(0.0f, 1.0f, 0.0f, 0.0f); // �𷺼� ����Ʈ
	cb.lightColor = Color(1, 1, 1, 1); // �𷺼� ����Ʈ
	cb.ambientColor = Color(0.2f, 0.2f, 0.2f, 0.2f); // ȯ�� ����Ʈ

	cb.spotPosition = float3(0.0f, 0.0f, 2.0f); // ����
	cb.spotDirection = float3(0.0f, 0.0f, -1.0f); // ����
	cb.spotRange = 5.0f;         // ����
	cb.spotAngle = cosf(ConvertDegToRad(20.0f));         // ����

	// ����Ʈ
	cb.pointPosition = float3(0.0f, 0.0f, 2.0f);
	cb.pointLightRange = 2.1f;
	cb.attenuationConst = 1.0f;
	cb.attenuationLinear = 0.09f;
	cb.attenuationQuad = 0.032f;

	material_->Setting();

	layout_->Setting();
	vertex_->Setting();

	rasterizer_->SetFillMode(FillMode::Solid);
	rasterizer_->Setting();

	constantBuffer_->Update(&cb);
	constantBuffer_->VSSetting(0);
	constantBuffer_->PSSetting(0);

	vertex_->Draw();
}
