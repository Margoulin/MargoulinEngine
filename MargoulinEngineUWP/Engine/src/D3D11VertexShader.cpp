#include "D3D11VertexShader.hpp"

#include <d3dcompiler.h>
#include "Logger.hpp"
#include "String.hpp"

auto	D3D11VertexShader::InitializeDefaultVertexShader(ID3D11Device* device) -> void
{
	HRESULT hr = S_OK;

	MString vertexShader =
		"cbuffer ViewProjectionConstantBuffer : register(b0)\
		{\
			matrix view;\
			matrix projection;\
		};\
		\
		cbuffer ModelConstantBuffer : register(b1)\
		{\
			matrix model;\
		};\
		cbuffer UnlitColorBuffer : register(b2)\
		{\
			float4	unlitColor;\
		};\
		\
		struct VS_INPUT \
		{ \
			float3 a_position : POSITION; \
		}; \
		\
		struct PS_INPUT \
		{ \
			float4 pos : SV_POSITION; \
			float4 color : COLOR0; \
		}; \
		\
		PS_INPUT main(VS_INPUT input)\
		{ \
			PS_INPUT output;\
			float4 tempPos = float4(input.a_position, 1.0f);\
			tempPos = mul(tempPos, model);\
			tempPos = mul(tempPos, view);\
			tempPos = mul(tempPos, projection);\
			output.pos = tempPos;\
			output.color = unlitColor; \
			return output;\
		}";

	ID3D10Blob* errorBlob = nullptr;
	D3DCompile(vertexShader.Str(), vertexShader.Count(), nullptr, nullptr, nullptr
		, "main", VsTargets[FeatureLevelIndex], 0, 0, &shaderBlob, &errorBlob);
	if (!shaderBlob) 
	{
		Logger::Log((char*)errorBlob->GetBufferPointer());
		//OutputDebugString((LPCSTR)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), 
		shaderBlob->GetBufferSize(), 
		nullptr, &vtxShader);
}

auto	D3D11VertexShader::InitializeTextureVertexShader(ID3D11Device* device) -> void
{
	HRESULT hr = S_OK;
	MString vertexShader =
		"cbuffer ViewProjectionConstantBuffer : register(b0)\
		{\
			matrix view;\
			matrix projection;\
		};\
		\
        struct VS_INPUT\
        {\
            float3 pos : POSITION;\
            float2 uv  : TEXCOORD0;\
        };\
        \
        struct PS_INPUT\
        {\
            float4 pos : SV_POSITION;\
            float2 uv  : TEXCOORD0;\
        };\
        \
        PS_INPUT main(VS_INPUT input)\
        {\
            PS_INPUT output;\
            output.pos = mul(float4(input.pos.xyz, 1.f), projection);\
            output.uv  = input.uv;\
            return output;\
        }";

	ID3D10Blob* errorBlob = nullptr;
	D3DCompile(vertexShader.Str(), vertexShader.Count(), nullptr, nullptr, nullptr
		, "main", VsTargets[FeatureLevelIndex], 0, 0, &shaderBlob, &errorBlob);
	if (!shaderBlob)
	{
		//Logger::Log(MString((char*)errorBlob->GetBufferPointer()));
		hr = E_FAIL;
		errorBlob->Release();
	}
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vtxShader);
}