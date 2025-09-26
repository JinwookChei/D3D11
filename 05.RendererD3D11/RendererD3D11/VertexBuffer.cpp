#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
	: refCount_(1),
	stride_(0),
	offset_(0),
	indexCount_(0),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr)
{
}

VertexBuffer::~VertexBuffer()
{
	Cleanup();
}

HRESULT __stdcall VertexBuffer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall VertexBuffer::AddRef() {
	++refCount_;
	return refCount_;
}

ULONG __stdcall VertexBuffer::Release() {
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall VertexBuffer::Setting()
{
	UINT offset = 0;
	GRenderer->Context()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride_, &offset);

	// 인덱스 버퍼 설정
	GRenderer->Context()->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	// 프리미티브 유형 설정
	GRenderer->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool __stdcall VertexBuffer::Draw()
{
	GRenderer->Context()->DrawIndexed(indexCount_, 0, 0);

	GRenderer->IncrementDrawCall();
	return true;
}

const std::vector<D3D11_INPUT_ELEMENT_DESC>& VertexBuffer::GetDesc() const
{
	return desc_;
}

bool VertexBuffer::Initialize(void* vertices, UINT vertexSize, UINT vertexCount, void* indices, UINT indexTypeSize /*= 0*/, UINT indexCount /*= 0*/)
{
	if (nullptr == vertices)
	{
		__debugbreak();
		return false;
	}
	if (0 == vertexSize || 0 == vertexCount)
	{
		__debugbreak();
		return false;
	}

	stride_ = vertexSize;

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexSize * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = vertices;

	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &vertexBuffer_);
	if (FAILED(hr))
	{
		__debugbreak();
		return false;
	}

	if (nullptr == indices && 0 != indexCount)
	{
		__debugbreak();
		return false;
	}

	if (nullptr == indices)
	{
		return true;
	}

	if (0 == indexCount)
	{
		__debugbreak();
		return false;
	}

	// 인덱스 버퍼
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = indexTypeSize * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = indices;

	hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &indexBuffer_);
	if (FAILED(hr))
	{
		__debugbreak();
		return false;
	}

	indexCount_ = indexCount;

	return true;
}

void VertexBuffer::Cleanup()
{
	if (nullptr != indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
	if (nullptr != vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
}

bool VertexBuffer::AddInputLayOut(const char* semanticName, uint32_t format, bool isInstanceData, uint32_t inputSlot, uint32_t semanticIndex)
{
	D3D11_INPUT_ELEMENT_DESC data = { 0 };

	data.InputSlotClass = isInstanceData ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;

	data.SemanticName = semanticName;
	data.SemanticIndex = semanticIndex;
	data.Format = (DXGI_FORMAT)format;

	data.InputSlot = inputSlot;

	data.AlignedByteOffset = offset_;
	offset_ += FormatSize(data.Format);

	if (isInstanceData)
	{
		data.InstanceDataStepRate = 1;
	}
	else
	{
		data.InstanceDataStepRate = 0;
	}

	desc_.push_back(data);

	return true;
}

UINT VertexBuffer::FormatSize(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_UNKNOWN:
		return -1;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 12;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return 8;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return 4;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
		return 2;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		return 1;
	case DXGI_FORMAT_R1_UNORM:
		break;
		/*case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
			break;
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
			break;
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
			break;
		case DXGI_FORMAT_BC1_TYPELESS:
			break;
		case DXGI_FORMAT_BC1_UNORM:
			break;
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC2_TYPELESS:
			break;
		case DXGI_FORMAT_BC2_UNORM:
			break;
		case DXGI_FORMAT_BC2_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC3_TYPELESS:
			break;
		case DXGI_FORMAT_BC3_UNORM:
			break;
		case DXGI_FORMAT_BC3_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC4_TYPELESS:
			break;
		case DXGI_FORMAT_BC4_UNORM:
			break;
		case DXGI_FORMAT_BC4_SNORM:
			break;
		case DXGI_FORMAT_BC5_TYPELESS:
			break;
		case DXGI_FORMAT_BC5_UNORM:
			break;
		case DXGI_FORMAT_BC5_SNORM:
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			break;
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			break;
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			break;
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC6H_TYPELESS:
			break;
		case DXGI_FORMAT_BC6H_UF16:
			break;
		case DXGI_FORMAT_BC6H_SF16:
			break;
		case DXGI_FORMAT_BC7_TYPELESS:
			break;
		case DXGI_FORMAT_BC7_UNORM:
			break;
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			break;
		case DXGI_FORMAT_AYUV:
			break;
		case DXGI_FORMAT_Y410:
			break;
		case DXGI_FORMAT_Y416:
			break;
		case DXGI_FORMAT_NV12:
			break;
		case DXGI_FORMAT_P010:
			break;
		case DXGI_FORMAT_P016:
			break;
		case DXGI_FORMAT_420_OPAQUE:
			break;
		case DXGI_FORMAT_YUY2:
			break;
		case DXGI_FORMAT_Y210:
			break;
		case DXGI_FORMAT_Y216:
			break;
		case DXGI_FORMAT_NV11:
			break;
		case DXGI_FORMAT_AI44:
			break;
		case DXGI_FORMAT_IA44:
			break;
		case DXGI_FORMAT_P8:
			break;
		case DXGI_FORMAT_A8P8:
			break;
		case DXGI_FORMAT_B4G4R4A4_UNORM:
			break;
		case DXGI_FORMAT_P208:
			break;
		case DXGI_FORMAT_V208:
			break;
		case DXGI_FORMAT_V408:
			break;
		case DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
			break;
		case DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
			break;
		case DXGI_FORMAT_FORCE_UINT:
			break;*/
	default:
		return -1;
	}
	return -1;
}