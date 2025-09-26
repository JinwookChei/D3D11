#pragma once

class Texture final :
	public ITexture {
public:
	Texture();

	~Texture();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	float2 Size() const;

	bool SetTexture(ID3D11Texture2D* texture);

	ID3D11RenderTargetView* RenderTargetView();

	ID3D11DepthStencilView* DepthStencilView();

	void BindTargetTextureForPixel(unsigned int slot);

	static Texture* Create(const float2& size, DXGI_FORMAT format, uint32_t flag);

	static Texture* Create(const D3D11_TEXTURE2D_DESC& desc);

private:
	bool CreateRenderTargetView();

	bool CreateDepthStencilView();

	bool CreateShaderResourceView();

	void Cleanup();

	ULONG refCount_;

	ID3D11Texture2D* texture_;

	ID3D11RenderTargetView* renderTargetView_;
	ID3D11DepthStencilView* depthStencilView_;
	ID3D11ShaderResourceView* shaderResourceView_;

	D3D11_TEXTURE2D_DESC desc_;
};