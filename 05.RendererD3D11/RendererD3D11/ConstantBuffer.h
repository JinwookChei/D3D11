#pragma once

class ConstantBuffer final
	: public IConstantBuffer
{
public:
	ConstantBuffer();

	~ConstantBuffer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Update(void* srcData) override;

	void __stdcall VSSetting(unsigned int slot) override;

	void __stdcall PSSetting(unsigned int slot) override;

	bool CreateBuffer(unsigned int bufferSize);

private:
	void Cleanup();

	ULONG refCount_;

	ID3D11Buffer* buffer_;
};