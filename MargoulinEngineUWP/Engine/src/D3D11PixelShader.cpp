#include "D3D11PixelShader.hpp"

#include <d3dcompiler.h>
#include "Logger.hpp"
#include "String.hpp"
#include "D3D11VertexShader.hpp"

auto	D3D11PixelShader::InitializeUnlitColorPixelShader(ID3D11Device* device, D3D11VertexShader* vtxShader) -> void
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(inLayoutDesc, _countof(inLayoutDesc), vtxShader->GetShaderBlob()->GetBufferPointer(), 
		vtxShader->GetShaderBlob()->GetBufferSize(), &inLayout);

	MString pixelShader =
		"struct PS_INPUT \
		{ \
			float4 pos : SV_POSITION; \
			float4 color : COLOR0; \
		}; \
		\
		float4 main(PS_INPUT input) : SV_Target\
		{ \
			return input.color; \
		} \
		";

	ID3D10Blob* shaderBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;
	D3DCompile(pixelShader.Str(), pixelShader.Count(), nullptr, nullptr, nullptr
		, "main", PsTargets[FeatureLevelIndex], 0, 0, &shaderBlob, &errorBlob);
	if (!shaderBlob) 
	{
		//OutputDebugString((LPCSTR)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), 
		shaderBlob->GetBufferSize(), 
		nullptr, &pxShader);
	SAFE_RELEASE(shaderBlob);
}
	
auto	D3D11PixelShader::InitializeTexturePixelShader(ID3D11Device* device, D3D11VertexShader* vtxShader) -> void
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(inLayoutDesc, _countof(inLayoutDesc), vtxShader->GetShaderBlob()->GetBufferPointer(),
		vtxShader->GetShaderBlob()->GetBufferSize(), &inLayout);

	MString pixelShader =
		"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = texture0.Sample(sampler0, input.uv);  \
			return out_col; \
            }";

	ID3D10Blob* tempShaderBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;
	D3DCompile(pixelShader.Str(), pixelShader.Count(), nullptr, nullptr, nullptr
		, "main", PsTargets[FeatureLevelIndex], 0, 0, &tempShaderBlob, &errorBlob);
	if (!tempShaderBlob)
	{
		//OutputDebugString((LPCSTR)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}
	device->CreatePixelShader(tempShaderBlob->GetBufferPointer(), tempShaderBlob->GetBufferSize(), nullptr, &pxShader);
	SAFE_RELEASE(tempShaderBlob);
}

auto	D3D11PixelShader::Shutdown() -> void
{
	*inLayout.ReleaseAndGetAddressOf() = nullptr;
	*pxShader.ReleaseAndGetAddressOf() = nullptr;
}
	
auto	D3D11PixelShader::Bind(ID3D11DeviceContext* context) const -> void
{
	context->PSSetShader(pxShader.Get(), nullptr, 0);
	context->IASetInputLayout(inLayout.Get());
}