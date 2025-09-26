#include "stdafx.h"
#include "Camera.h"
#include "Test.h"

Camera::Camera()
	: size_(0.0f, 0.0f),
	fov_(60.f),
	near_(0.1f),
	far_(10000.0f),
	clearColor_(0.0f, 0.0f, 0.0f, 0.0f),
	renderTarget_(nullptr),
	vertex_(nullptr),
	layout_(nullptr),
	constantBuffer_(nullptr),
	rasterizer_(nullptr),
	material_(nullptr)
{
	MatrixIdentity(view_);
	MatrixIdentity(projection_);
}

Camera::~Camera()
{
	if (nullptr != renderTarget_)
	{
		renderTarget_->Release();
		renderTarget_ = nullptr;
	}
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

void Camera::BeginPlay()
{
	/*Transform& transform = GetTransform();

	transform.SetPosition({ -5.0f, 0.0f, 5.0f, 1.0f });

	transform.AddRotaionY(40);
	transform.AddRotaionX(120);*/

	renderTarget_ = GRenderer->CreateRenderTarget(size_, clearColor_);
}

void Camera::Update()
{

}

void Camera::Render()
{
	CameraTransformUpdate();

	renderTarget_->Clear();

	renderTarget_->Setting();
}

void Camera::SetSize(const float2& size)
{
	size_ = size;

	if (nullptr != renderTarget_)
	{
		renderTarget_->Release();
		renderTarget_ = nullptr;
	}

	renderTarget_ = GRenderer->CreateRenderTarget(size_, clearColor_);


	std::vector<MergeVertex> sphereVertices;
	std::vector<WORD> index;
	CreateRect(&sphereVertices, &index);

	vertex_ = GRenderer->CreateVertex(sphereVertices.data(), (UINT)sizeof(MergeVertex), (UINT)sphereVertices.size(), index.data(), (UINT)sizeof(WORD), (UINT)index.size());

	vertex_->AddInputLayOut("POSITION", 16, false, 0, 0);
	vertex_->AddInputLayOut("TEXCOORD", 16, false, 0, 0);

	IShader* vertexShader = GRenderer->CreateShader(ShaderType::Vertex, L"VertexMergeShader.cso");
	IShader* pixelShader = GRenderer->CreateShader(ShaderType::Pixel, L"PixelMergeShader.cso");
	ISamplerState* sampler = GRenderer->CreateSampler(true, false);

	layout_ = GRenderer->CreateLayOut(vertex_, vertexShader);

	material_ = GRenderer->CreateMaterial();

	constantBuffer_ = GRenderer->CreateConstantBuffer((unsigned int)sizeof(MergeConstantBuffer));

	rasterizer_ = GRenderer->CreateRasterizer(true);

	material_->SetVertexShader(vertexShader);
	material_->SetPixelShader(pixelShader);
	material_->SetSampler(sampler, 0);

	vertexShader->Release();
	pixelShader->Release();
	sampler->Release();
}

void Camera::SetConfig(float fov, float Near, float Far)
{
	fov_ = fov;
	near_ = Near;
	far_ = Far;
}

void Camera::SetClearColor(const Color& clearColor)
{
	clearColor_ = clearColor;
}

const float4x4& Camera::View() const
{
	return view_;
}

const float4x4& Camera::Projection() const
{
	return projection_;
}

void Camera::Merge(const float2& Offset, const float2& Scale)
{
	material_->Setting();

	renderTarget_->BindTargetTextureForPixel(0);

	layout_->Setting();
	vertex_->Setting();

	rasterizer_->SetFillMode(FillMode::Solid);
	rasterizer_->Setting();

	MergeConstantBuffer cb;
	cb.Offset = Offset;
	cb.Scale = Scale;

	constantBuffer_->Update(&cb);
	constantBuffer_->VSSetting(0);

	vertex_->Draw();

	renderTarget_->ClearTargetTexutreForPixel(0);
}

void Camera::CameraTransformUpdate()
{
	Transform& transform = GetTransform();

	float4 eyeDir = transform.ForwardVector();
	float4 eyeUp = transform.UpVector();
	float4 eyePos = transform.GetPosition();

	MatrixLookToLH(view_, eyePos, eyeDir, eyeUp);

	MatrixPerspectiveFovLH(projection_, fov_, size_.X / size_.Y, near_, far_);
}
