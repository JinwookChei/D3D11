#pragma once

enum class ShaderType
{
	Vertex = 0,
	Pixel
};

enum class FillMode
{
	WireFrame = 0,
	Solid
};

struct ITexture : public IUnknown {
};

struct IRenderTarget : public IUnknown {
	virtual void Clear() = 0;
	virtual void Setting() = 0;
	virtual void BindTargetTextureForPixel(unsigned int slot) = 0;
	virtual void ClearTargetTexutreForPixel(unsigned int slot) = 0;
};

struct IVertex : public IUnknown {
	virtual void __stdcall Setting() = 0;
	virtual bool __stdcall AddInputLayOut(const char* semanticName, unsigned int format, bool isInstanceData, unsigned int inputSlot, unsigned int semanticIndex) = 0;
	virtual bool __stdcall Draw() = 0;
};

struct IShader : public IUnknown {
};

struct ISamplerState : public IUnknown {
	virtual void __stdcall Setting(unsigned int slot) = 0;
};

struct IMaterial : public IUnknown {
	virtual void __stdcall SetVertexShader(IShader* shader) = 0;
	virtual void __stdcall SetPixelShader(IShader* shader) = 0;
	virtual void __stdcall SetSampler(ISamplerState* sampler, unsigned int slot = 0) = 0;

	virtual void __stdcall Setting() = 0;
};

struct IInputLayOut : public IUnknown {
	virtual void __stdcall Setting() = 0;
};

struct IConstantBuffer : public IUnknown {
	virtual void __stdcall Update(void* srcData) = 0;
	virtual void __stdcall VSSetting(unsigned int slot) = 0;
	virtual void __stdcall PSSetting(unsigned int slot) = 0;
};

struct IRasterizer : public IUnknown {
	virtual void __stdcall Setting() = 0;
	virtual void __stdcall SetFillMode(FillMode fillmode) = 0;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, UINT width, UINT height) = 0;

	virtual void __stdcall BeginRender() = 0;

	virtual void __stdcall EndRender() = 0;

	virtual unsigned __int64 __stdcall DrawCallCount() = 0;

	virtual IVertex* CreateVertex(void* vertices, UINT vertexSize, UINT vertexCount, void* indices = nullptr, UINT indexTypeSize = 0, UINT indexCount = 0) = 0;

	virtual IShader* __stdcall CreateShader(ShaderType type, const wchar_t* path) = 0;

	virtual IInputLayOut* CreateLayOut(IVertex* vertex, IShader* vertexShader) = 0;

	virtual ISamplerState* CreateSampler(bool linear, bool clamp) = 0;

	virtual IMaterial* CreateMaterial() = 0;

	virtual IConstantBuffer* CreateConstantBuffer(unsigned int bufferSize) = 0;

	virtual IRasterizer* CreateRasterizer(bool back) = 0;

	virtual IRenderTarget* CreateRenderTarget(const float2& size, const Color& clearColor) = 0;
};