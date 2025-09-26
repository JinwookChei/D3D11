#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	: refCount_(1),
	texture_(nullptr),
	renderTargetView_(nullptr),
	depthStencilView_(nullptr),
	shaderResourceView_(nullptr),
	desc_()
{
}

Texture::~Texture()
{
	Cleanup();
}

HRESULT __stdcall Texture::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Texture::AddRef()
{
	++refCount_;
	return refCount_;
}

ULONG __stdcall Texture::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

float2 Texture::Size() const
{
	return float2((float)desc_.Width, (float)desc_.Height);
}

bool Texture::SetTexture(ID3D11Texture2D* texture)
{
	Cleanup();

	texture_ = texture;
	if (nullptr == texture_)
	{
		return true;
	}

	texture_->GetDesc(&desc_);

	if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & desc_.BindFlags &&
		false == CreateRenderTargetView())
	{
		return false;
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & desc_.BindFlags &&
		false == CreateDepthStencilView())
	{
		return false;
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & desc_.BindFlags &&
		false == CreateShaderResourceView())
	{
		return false;
	}

	return true;
}

bool Texture::CreateRenderTargetView()
{
	if (nullptr == texture_)
	{
		return false;
	}

	if (nullptr != renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	}

	HRESULT hr = GRenderer->Device()->CreateRenderTargetView(texture_, nullptr, &renderTargetView_);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Texture::CreateDepthStencilView()
{
	if (nullptr == texture_)
	{
		return false;
	}

	if (nullptr != depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}

	HRESULT hr = GRenderer->Device()->CreateDepthStencilView(texture_, nullptr, &depthStencilView_);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Texture::CreateShaderResourceView()
{
	if (nullptr == texture_)
	{
		return false;
	}
	if (nullptr != shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = nullptr;
	}

	HRESULT hr = GRenderer->Device()->CreateShaderResourceView(texture_, nullptr, &shaderResourceView_);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

ID3D11RenderTargetView* Texture::RenderTargetView()
{
	return renderTargetView_;
}

ID3D11DepthStencilView* Texture::DepthStencilView()
{
	return depthStencilView_;
}

void Texture::BindTargetTextureForPixel(unsigned int slot)
{
	GRenderer->Context()->PSSetShaderResources(slot, 1, &shaderResourceView_);
}

Texture* Texture::Create(const float2& size, DXGI_FORMAT format, uint32_t flag)
{
	D3D11_TEXTURE2D_DESC desc = { 0, };

	desc.ArraySize = 1;
	desc.Width = (UINT)size.X;
	desc.Height = (UINT)size.Y;

	desc.Format = format;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.MipLevels = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;

	desc.CPUAccessFlags = 0;
	desc.BindFlags = flag;

	return Create(desc);
}

Texture* Texture::Create(const D3D11_TEXTURE2D_DESC& desc)
{
	ID3D11Texture2D* texture = nullptr;

	HRESULT hr = GRenderer->Device()->CreateTexture2D(&desc, nullptr, &texture);
	if (FAILED(hr))
	{
		return nullptr;
	}

	Texture* newTexture = new Texture;
	newTexture->SetTexture(texture);
	return newTexture;
}

void Texture::Cleanup()
{
	if (nullptr != renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	}
	if (nullptr != depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}
	if (nullptr != shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = nullptr;
	}
	if (nullptr != texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
}
