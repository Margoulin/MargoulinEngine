#include "Shader.hpp"

#include <d3dcompiler.h>
#include "DirectXHelper.hpp"
#include <string>
#include "Logger.hpp"

static const D3D_FEATURE_LEVEL featureLevelArray[] = { D3D_FEATURE_LEVEL_11_0
, D3D_FEATURE_LEVEL_10_1
, D3D_FEATURE_LEVEL_10_0
, D3D_FEATURE_LEVEL_9_3 };
static int g_FeatureLevelIndex = 2;
static const char* g_VsTarget[] = { "vs_5_0", "vs_4_1", "vs_4_0", "vs_3_0" };
//static const char* g_VsTarget[] = { "vs_5_0", "vs_4_1", "vs_4_0", "vs_3_0" };
//static const char* g_PsTarget[] = { "ps_5_0", "ps_4_1", "ps_4_0", "ps_3_0" };
static const char* g_PsTarget[] = { "ps_5_0", "ps_4_1", "ps_4_0", "ps_3_0" };

auto	Shader::Initialize(ID3D11Device* device) -> void
{
	HRESULT hr = S_OK;
	std::string vertexShader =
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
	D3DCompile(vertexShader.c_str(), vertexShader.size(), nullptr, nullptr, nullptr
		, "main", g_VsTarget[g_FeatureLevelIndex], 0, 0, &g_ShaderBlob, &errorBlob);
	if (!g_ShaderBlob) {
		Logger::Log(std::string((char*)errorBlob->GetBufferPointer()));
		//OutputDebugString((LPCSTR)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}
	device->CreateVertexShader(g_ShaderBlob->GetBufferPointer()
		, g_ShaderBlob->GetBufferSize()
		, nullptr, &g_VertexShader);


	D3D11_INPUT_ELEMENT_DESC vaoDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(vaoDesc, _countof(vaoDesc), g_ShaderBlob->GetBufferPointer()
		, g_ShaderBlob->GetBufferSize(), &g_VAO);

	SAFE_RELEASE(g_ShaderBlob);

	static const char* pixelShader =
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

	D3DCompile(pixelShader, strlen(pixelShader), nullptr, nullptr, nullptr
		, "main", g_PsTarget[g_FeatureLevelIndex], 0, 0, &g_ShaderBlob, &errorBlob);
	if (!g_ShaderBlob) {
		//OutputDebugString((LPCSTR)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}
	device->CreatePixelShader(g_ShaderBlob->GetBufferPointer()
		, g_ShaderBlob->GetBufferSize()
		, nullptr, &g_PixelShader);
	SAFE_RELEASE(g_ShaderBlob);

	CD3D11_BUFFER_DESC unlitColorConstantBufferDesc(sizeof(DirectX::XMFLOAT4), D3D11_BIND_CONSTANT_BUFFER);
	device->CreateBuffer(&unlitColorConstantBufferDesc, nullptr, &unlitColorConstantBuffer);
}

auto Shader::Shutdown() -> void
{
	SAFE_RELEASE(g_VAO);
	SAFE_RELEASE(g_ShaderBlob);
	SAFE_RELEASE(g_PixelShader);
	SAFE_RELEASE(g_VertexShader);
	*unlitColorConstantBuffer.ReleaseAndGetAddressOf() = nullptr;
}

auto	Shader::SetCurrent(ID3D11DeviceContext* context) -> void
{
	context->VSSetShader(g_VertexShader, nullptr, 0);
	context->PSSetShader(g_PixelShader, nullptr, 0);

	context->IASetInputLayout(g_VAO);

	context->UpdateSubresource(unlitColorConstantBuffer.Get(), 0, NULL,
		&unlitColorData, 0, 0);
	context->VSSetConstantBuffers(2, 1, unlitColorConstantBuffer.GetAddressOf());
}

#include <imgui.h>

#ifdef _DEBUG

auto	Shader::ImGuiUpdate() -> void
{
}

#endif // _DEBUG