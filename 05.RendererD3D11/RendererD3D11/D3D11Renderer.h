#pragma once

class RenderTarget;
class D3D11Renderer final
	: public IRenderer
{
public:
	D3D11Renderer();

	~D3D11Renderer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall Initialize(void* hWnd, UINT width, UINT height) override;

	void __stdcall BeginRender() override;

	void __stdcall EndRender() override;

	unsigned __int64 __stdcall DrawCallCount() override;

	IVertex* CreateVertex(void* vertices, UINT vertexSize, UINT vertexCount, void* indices = nullptr, UINT indexTypeSize = 0, UINT indexCount = 0) override;

	IShader* __stdcall CreateShader(ShaderType type, const wchar_t* path) override;

	IInputLayOut* CreateLayOut(IVertex* vertex, IShader* vertexShader) override;

	ISamplerState* CreateSampler(bool linear, bool clamp) override;

	IMaterial* CreateMaterial() override;

	IConstantBuffer* CreateConstantBuffer(unsigned int bufferSize) override;

	IRasterizer* CreateRasterizer(bool back) override;

	IRenderTarget* CreateRenderTarget(const float2& size, const Color& clearColor) override;

	ID3D11Device* Device();

	ID3D11DeviceContext* Context();

	void IncrementDrawCall();

private:
	IDXGIAdapter* GetBestAdapter();

	bool CreateSwapChain(UINT width, UINT height);

	bool CreateRenderTarget();

	void Cleanup();

	bool coInit_;

	ULONG refCount_;

	unsigned __int64 drawCallCount_;

	HWND hWnd_;

	ID3D11Device* device_;

	ID3D11DeviceContext* context_;

	IDXGISwapChain* swapChain_;

	RenderTarget* backBuffer_;
};