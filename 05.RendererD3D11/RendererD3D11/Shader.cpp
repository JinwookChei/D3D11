#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
	: refCount_(1)
{
}

HRESULT __stdcall Shader::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Shader::AddRef() {
	++refCount_;
	return refCount_;
}

ULONG __stdcall Shader::Release() {
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

bool Shader::CreateShader(ID3DBlob* blob)
{
	if (nullptr == blob)
	{
		return false;
	}

	blob_ = blob;

	blob_->AddRef();

	return OnCreateShader(blob_);
}

void Shader::Cleanup()
{
	OnCleanup();

	if (nullptr != blob_)
	{
		blob_->Release();
		blob_ = nullptr;
	}
}

void* Shader::GetBufferPointer()
{
	if (nullptr == blob_)
	{
		return nullptr;
	}
	return blob_->GetBufferPointer();
}

size_t Shader::GetBufferSize()
{
	if (nullptr == blob_)
	{
		return 0;
	}
	return blob_->GetBufferSize();
}

bool VertexShader::OnCreateShader(ID3DBlob* blob)
{
	if (nullptr == blob)
	{
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader_);
	if (FAILED(hr))
	{
		__debugbreak();
		return false;
	}

	return true;
}

void VertexShader::SetShader()
{
	GRenderer->Context()->VSSetShader(shader_, nullptr, 0);
}

void VertexShader::OnCleanup()
{
	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}

bool PixelShader::OnCreateShader(ID3DBlob* blob)
{
	if (nullptr == blob)
	{
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader_);
	if (FAILED(hr))
	{
		__debugbreak();
		return false;
	}

	return true;
}

void PixelShader::SetShader()
{
	GRenderer->Context()->PSSetShader(shader_, nullptr, 0);
}

void PixelShader::OnCleanup()
{
	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}
