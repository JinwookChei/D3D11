#include "stdafx.h"
#include "Material.h"
#include "Shader.h"

Material::Material()
	:refCount_(1),
	samplerSlot_(0),
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	sampler_(nullptr)
{
}

Material::~Material()
{
	Cleanup();
}

HRESULT __stdcall Material::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Material::AddRef()
{
	++refCount_;
	return refCount_;
}

ULONG __stdcall Material::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall Material::SetVertexShader(IShader* shader)
{
	if (nullptr != vertexShader_)
	{
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}

	vertexShader_ = (Shader*)shader;

	if (nullptr != vertexShader_)
	{
		vertexShader_->AddRef();
	}
}

void __stdcall Material::SetPixelShader(IShader* shader)
{
	if (nullptr != pixelShader_)
	{
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}

	pixelShader_ = (Shader*)shader;

	if (nullptr != pixelShader_)
	{
		pixelShader_->AddRef();
	}
}

void __stdcall Material::SetSampler(ISamplerState* sampler, unsigned int slot)
{
	if (nullptr != sampler_)
	{
		sampler_->Release();
		sampler_ = nullptr;
	}

	sampler_ = sampler;

	if (nullptr != sampler_)
	{
		sampler_->AddRef();
	}

	samplerSlot_ = slot;
}

void __stdcall Material::Setting()
{
	if (nullptr != vertexShader_)
	{
		vertexShader_->SetShader();
	}
	if (nullptr != pixelShader_)
	{
		pixelShader_->SetShader();
	}
	if (nullptr != sampler_)
	{
		sampler_->Setting(samplerSlot_);
	}
}

void Material::Cleanup()
{
	if (nullptr != vertexShader_)
	{
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}
	if (nullptr != pixelShader_)
	{
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}
	if (nullptr != sampler_)
	{
		sampler_->Release();
		sampler_ = nullptr;
	}
}
