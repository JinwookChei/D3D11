#pragma once

class VertexBuffer final
	: public IVertex {
	friend class D3D11Renderer;
public:
	VertexBuffer();

	~VertexBuffer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall AddInputLayOut(const char* semanticName, uint32_t format, bool isInstanceData, uint32_t inputSlot, uint32_t semanticIndex) override;

	void __stdcall Setting() override;

	bool __stdcall Draw() override;

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDesc() const;

private:
	bool Initialize(void* vertices, UINT vertexSize, UINT vertexCount, void* indices = nullptr, UINT indexTypeSize = 0, UINT indexCount = 0);

	void Cleanup();

	UINT FormatSize(DXGI_FORMAT format);

	ULONG refCount_;

	UINT stride_;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	int offset_;

	UINT indexCount_;

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc_;
};