#pragma once

class Texture;
class RenderTarget final
	:public IRenderTarget {
public:
	RenderTarget();

	~RenderTarget();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Clear() override;

	void Setting() override;

	void BindTargetTextureForPixel(unsigned int slot) override;

	void ClearTargetTexutreForPixel(unsigned int slot) override;

	bool SetTexture(Texture* texture);

	void SetClearColor(const Color& clearColor);

	bool CreateDepthTexture();

private:
	void Cleanup();

	ULONG refCount_;

	Texture* texture_;

	Texture* depthTexture_;

	D3D11_VIEWPORT viewport_;

	Color clearColor_;
};