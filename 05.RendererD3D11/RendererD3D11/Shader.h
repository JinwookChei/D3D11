#pragma once

class Shader : public IShader {
public:
	Shader();

	virtual ~Shader() = 0 {
	}

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	virtual void SetShader() = 0;

	bool CreateShader(ID3DBlob* blob);

	void Cleanup();

	void* GetBufferPointer();

	size_t GetBufferSize();

protected:
	virtual bool OnCreateShader(ID3DBlob* blob) = 0;

	virtual void OnCleanup() = 0;

private:
	ULONG refCount_;

	ID3DBlob* blob_;
};

class VertexShader final
	: public Shader
{
public:
	VertexShader()
		: shader_(nullptr)
	{
	}

	~VertexShader()
	{
		Cleanup();
	}

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* blob) override;

	void OnCleanup() override;

private:
	ID3D11VertexShader* shader_;
};

class PixelShader final
	: public Shader
{
public:
	PixelShader()
		: shader_(nullptr)
	{
	}

	~PixelShader()
	{
		Cleanup();
	}

	void SetShader() override;


protected:
	bool OnCreateShader(ID3DBlob* blob) override;

	void OnCleanup() override;

private:
	ID3D11PixelShader* shader_;
};