#pragma once

class Shader;
class Material final
	: public IMaterial
{
public:
	Material();

	~Material();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall SetVertexShader(IShader* shader) override;
	void __stdcall SetPixelShader(IShader* shader) override;
	void __stdcall SetSampler(ISamplerState* sampler, unsigned int slot = 0) override;

	void __stdcall Setting() override;

private:
	void Cleanup();

	ULONG refCount_;

	unsigned int samplerSlot_;

	Shader* vertexShader_;

	Shader* pixelShader_;

	ISamplerState* sampler_;
};