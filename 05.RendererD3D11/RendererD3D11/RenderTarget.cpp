#include "stdafx.h"
#include "RenderTarget.h"
#include "Texture.h"

RenderTarget* GCurrentSetRenderTarget = nullptr;

RenderTarget::RenderTarget()
	: refCount_(1),
	texture_(nullptr),
	depthTexture_(nullptr),
	viewport_(),
	clearColor_(0.0f, 0.2f, 0.4f, 1.0f)
{
}

RenderTarget::~RenderTarget()
{
	Cleanup();
}

HRESULT __stdcall RenderTarget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall RenderTarget::AddRef()
{
	++refCount_;
	return refCount_;
}

ULONG __stdcall RenderTarget::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}
void RenderTarget::Clear()
{
	ID3D11RenderTargetView* view = texture_->RenderTargetView();
	if (nullptr == view)
	{
		return;
	}

	GRenderer->Context()->ClearRenderTargetView(view, clearColor_.Arr1D);

	if (nullptr != depthTexture_)
	{
		ID3D11DepthStencilView* depth = depthTexture_->DepthStencilView();
		if (nullptr != depth)
		{
			GRenderer->Context()->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
}

void RenderTarget::Setting()
{
	if (GCurrentSetRenderTarget == this)
	{
		return;
	}

	ID3D11RenderTargetView* view = texture_->RenderTargetView();
	if (nullptr == view)
	{
		return;
	}

	ID3D11DepthStencilView* depth = nullptr;
	if (nullptr != depthTexture_)
	{
		depth = depthTexture_->DepthStencilView();
	}

	GCurrentSetRenderTarget = this;

	GRenderer->Context()->OMSetRenderTargets(1, &view, depth);
	GRenderer->Context()->RSSetViewports(1, &viewport_);
}

void RenderTarget::BindTargetTextureForPixel(unsigned int slot)
{
	texture_->BindTargetTextureForPixel(slot);
}

void RenderTarget::ClearTargetTexutreForPixel(unsigned int slot)
{
	ID3D11ShaderResourceView* pNull = nullptr;

	GRenderer->Context()->PSSetShaderResources(slot, 1, &pNull);
}

bool RenderTarget::SetTexture(Texture* texture)
{
	Cleanup();

	if (nullptr == texture)
	{
		return true;
	}

	texture_ = texture;

	float2 size = texture_->Size();

	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.Width = size.X;
	viewport_.Height = size.Y;
	viewport_.MinDepth = 0;
	viewport_.MaxDepth = 1.0f;

	return true;
}

void RenderTarget::SetClearColor(const Color& clearColor)
{
	clearColor_ = clearColor;
}

bool RenderTarget::CreateDepthTexture()
{
	// 4바이트중 3바이트는 0~1.0f 표현하는 depth
	// 나머지 1바이트는 스텐실 로 사용
	depthTexture_ = Texture::Create(texture_->Size(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);

	return nullptr != depthTexture_;
}

void RenderTarget::Cleanup()
{
	if (nullptr != texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
	if (nullptr != depthTexture_)
	{
		depthTexture_->Release();
		depthTexture_ = nullptr;
	}
}
