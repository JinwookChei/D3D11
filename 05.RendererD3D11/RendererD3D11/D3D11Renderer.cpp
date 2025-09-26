#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "Shader.h"
#include "SamplerState.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Rasterizer.h"

D3D11Renderer* GRenderer = nullptr;

D3D11Renderer::D3D11Renderer()
	:coInit_(false),
	refCount_(1),
	drawCallCount_(0),
	hWnd_(nullptr),
	device_(nullptr),
	context_(nullptr),
	swapChain_(nullptr),
	backBuffer_(nullptr)
{
	GRenderer = this;
}

D3D11Renderer::~D3D11Renderer()
{
	Cleanup();
}

HRESULT __stdcall D3D11Renderer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11Renderer::AddRef() {
	++refCount_;
	return refCount_;
}

ULONG __stdcall D3D11Renderer::Release() {
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

bool __stdcall D3D11Renderer::Initialize(void* hWnd, UINT width, UINT height)
{
	if (nullptr == hWnd)
	{
		return false;
	}

	hWnd_ = (HWND)hWnd;

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return false;
	}
	coInit_ = true;

	IDXGIAdapter* pBestAdapter = GetBestAdapter();
	if (nullptr == pBestAdapter)
	{
		return false;
	}

	int flag = 0;

#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDevice(
		pBestAdapter,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		flag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&device_,
		&featureLevel,
		&context_);

	pBestAdapter->Release();
	pBestAdapter = nullptr;

	if (FAILED(hr))
	{
		return false;
	}

	if (featureLevel < D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0)
	{
		return false;
	}

	if (false == CreateSwapChain(width, height))
	{
		return false;
	}

	if (false == CreateRenderTarget())
	{
		return false;
	}

	return true;
}

IVertex* D3D11Renderer::CreateVertex(void* vertices, UINT vertexSize, UINT vertexCount, void* indices /*= nullptr*/, UINT indexTypeSize /*= 0*/, UINT indexCount /*= 0*/)
{
	VertexBuffer* vertexBuffer = new VertexBuffer;
	if (nullptr == vertexBuffer)
	{
		__debugbreak();
		return nullptr;
	}

	if (false == vertexBuffer->Initialize(vertices, vertexSize, vertexCount, indices, indexTypeSize, indexCount))
	{
		vertexBuffer->Release();
		return nullptr;
	}

	return vertexBuffer;
}

IShader* __stdcall D3D11Renderer::CreateShader(ShaderType type, const wchar_t* path)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr = D3DReadFileToBlob(path, &blob);
	if (FAILED(hr))
	{
		return nullptr;
	}

	Shader* shader = nullptr;
	switch (type)
	{
	case ShaderType::Vertex:
		shader = new VertexShader;
		break;
	case ShaderType::Pixel:
		shader = new PixelShader;
		break;
	}

	if (nullptr != shader && false == shader->CreateShader(blob))
	{
		shader->Release();
		shader = nullptr;
	}

	return shader;
}

IInputLayOut* D3D11Renderer::CreateLayOut(IVertex* vertex, IShader* vertexShader)
{
	InputLayout* newInputLayout = new InputLayout;
	if (false == newInputLayout->Create(vertex, vertexShader))
	{
		newInputLayout->Release();
		newInputLayout = nullptr;
	}

	return newInputLayout;
}

ISamplerState* D3D11Renderer::CreateSampler(bool linear, bool clamp)
{
	SamplerState* sampler = new SamplerState;
	if (false == sampler->CreateSampler(linear, clamp))
	{
		sampler->Release();
		sampler = nullptr;
	}

	return sampler;
}

IMaterial* D3D11Renderer::CreateMaterial()
{
	return new Material;
}

IConstantBuffer* D3D11Renderer::CreateConstantBuffer(unsigned int bufferSize)
{
	ConstantBuffer* buffer = new ConstantBuffer;
	if (false == buffer->CreateBuffer(bufferSize))
	{
		buffer->Release();
		buffer = nullptr;
	}
	return buffer;
}

IRasterizer* D3D11Renderer::CreateRasterizer(bool back)
{
	Rasterizer* rasterizer = new Rasterizer;
	if (false == rasterizer->CreateRasterizer(false, back))
	{
		rasterizer->Release();
		rasterizer = nullptr;
	}

	return rasterizer;
}

IRenderTarget* D3D11Renderer::CreateRenderTarget(const float2& size, const Color& clearColor)
{
	Texture* pTexture = Texture::Create(size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	if (nullptr == pTexture)
	{
		return nullptr;
	}

	RenderTarget* target = new RenderTarget;
	if (nullptr == target)
	{
		pTexture->Release();
		return nullptr;
	}
	if (false == target->SetTexture(pTexture))
	{
		target->Release();
		return nullptr;
	}

	target->SetClearColor(clearColor);

	return target;
}

void __stdcall D3D11Renderer::BeginRender()
{
	drawCallCount_ = 0;

	backBuffer_->Clear();

	backBuffer_->Setting();
}

void __stdcall D3D11Renderer::EndRender()
{
	swapChain_->Present(0, 0);
}

unsigned __int64 __stdcall D3D11Renderer::DrawCallCount()
{
	return drawCallCount_;
}

ID3D11Device* D3D11Renderer::Device()
{
	return device_;
}

ID3D11DeviceContext* D3D11Renderer::Context()
{
	return context_;
}

void D3D11Renderer::IncrementDrawCall()
{
	++drawCallCount_;
}

IDXGIAdapter* D3D11Renderer::GetBestAdapter()
{
	IDXGIFactory* pFactory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr))
	{
		return nullptr;
	}

	IDXGIAdapter* pBestAdapter = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	size_t maxDedicatedVedioMemory = 0;

	for (UINT n = 0; pFactory->EnumAdapters(n, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++n)
	{
		DXGI_ADAPTER_DESC desc;
		pAdapter->GetDesc(&desc);

		if (maxDedicatedVedioMemory < desc.DedicatedVideoMemory)
		{
			if (pBestAdapter)
			{
				pBestAdapter->Release();
			}
			pBestAdapter = pAdapter;
			maxDedicatedVedioMemory = desc.DedicatedVideoMemory;
		}
		else
		{
			pAdapter->Release();
		}
	}

	pFactory->Release();
	return pBestAdapter;
}

bool D3D11Renderer::CreateSwapChain(UINT width, UINT height)
{
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0x00, sizeof(sd));
	sd.BufferCount = 1; // 백 버퍼의 수
	sd.BufferDesc.Width = width; // 백 버퍼의 너비
	sd.BufferDesc.Height = height; // 백 버퍼의 높이
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 백 버퍼의 포맷
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 새로 고침 빈도 ( 분자 )
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 새로 고침 빈도 ( 분모 )
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백 버퍼의 용도
	sd.OutputWindow = hWnd_; // 렌더링 할 윈도우 핸들
	sd.SampleDesc.Count = 1; // 멀티샘플링 수
	sd.SampleDesc.Quality = 0; // 멀티샘플링 품질
	sd.Windowed = TRUE; // 창 모드 인지 아닌지.

	IDXGIDevice* swapDevice = nullptr;
	IDXGIAdapter* swapAdapter = nullptr;
	IDXGIFactory* swapFactory = nullptr;

	HRESULT hr = device_->QueryInterface(__uuidof(IDXGIDevice), (void**)&swapDevice);
	if (nullptr == swapDevice)
	{
		goto lb_return;
	}

	hr = swapDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&swapAdapter);
	if (nullptr == swapAdapter)
	{
		goto lb_return;
	}

	hr = swapAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&swapFactory);
	if (nullptr == swapFactory)
	{
		goto lb_return;
	}

	hr = swapFactory->CreateSwapChain(device_, &sd, &swapChain_);

lb_return:
	if (nullptr != swapDevice)
	{
		swapDevice->Release();
		swapDevice = nullptr;
	}
	if (nullptr != swapAdapter)
	{
		swapAdapter->Release();
		swapAdapter = nullptr;
	}
	if (nullptr != swapFactory)
	{
		swapFactory->Release();
		swapFactory = nullptr;
	}
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool D3D11Renderer::CreateRenderTarget()
{
	ID3D11Texture2D* backBufferTexture = nullptr;

	HRESULT hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if (FAILED(hr))
	{
		return false;
	}
	if (nullptr == backBufferTexture)
	{
		return false;
	}

	Texture* texture = new Texture;
	if (nullptr == texture)
	{
		backBufferTexture->Release();
		return false;
	}

	if (false == texture->SetTexture(backBufferTexture))
	{
		texture->Release();
		return false;
	}

	backBuffer_ = new RenderTarget;
	if (nullptr == backBuffer_)
	{
		texture->Release();
		return false;
	}

	if (false == backBuffer_->SetTexture(texture))
	{
		return false;
	}
	if (false == backBuffer_->CreateDepthTexture())
	{
		return false;
	}

	return true;
}

void D3D11Renderer::Cleanup()
{
	if (nullptr != context_)
	{
		context_->ClearState();
	}

	if (nullptr != backBuffer_)
	{
		backBuffer_->Release();
		backBuffer_ = nullptr;
	}
	if (nullptr != swapChain_)
	{
		swapChain_->Release();
		swapChain_ = nullptr;
	}
	if (nullptr != context_)
	{
		context_->Release();
		context_ = nullptr;
	}
	if (nullptr != device_)
	{
		device_->Release();
		device_ = nullptr;
	}
	if (coInit_)
	{
		CoUninitialize();
	}
}
