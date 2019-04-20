#include "D3D11ShaderFactory.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Logger.hpp"

#include "D3D11Context.hpp"
#include "D3D11VertexShader.hpp"
#include "D3D11PixelShader.hpp"


static int			FeatureLevelIndex = 2;
static char const* VsTargets[] = { "vs_5_0", "vs_4_1", "vs_4_0", "vs_3_0" };
static char const* PsTargets[] = { "ps_5_0", "ps_4_1", "ps_4_0", "ps_3_0" };

auto	D3D11ShaderFactory::CreateVertexTextureShader() -> Shader*
{
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
	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto* out = createVertexShader(vertexShader, inLayoutDesc, 2);
	out->SetShaderName("TEXTURE");
	return out;
}

auto	D3D11ShaderFactory::CreatePixelTextureShader() -> Shader*
{
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
	auto* out = createPixelShader(pixelShader);
	out->SetShaderName("TEXTURE");
	return out;
}

auto	D3D11ShaderFactory::CreateBasicVertexShader() -> Shader*
{
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
		cbuffer UnlitColorBuffer : register(b3)\
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
	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	auto* out = createVertexShader(vertexShader, inLayoutDesc, 1);
	out->SetShaderName("Basic");
	return out;
}

auto	D3D11ShaderFactory::CreateLineVertexShader() -> Shader*
{
	MString vertexShader =
		"cbuffer ViewProjectionConstantBuffer : register(b0)\
		{\
			matrix view;\
			matrix projection;\
		};\
		\
		struct PS_INPUT \
		{ \
			float4 pos : SV_POSITION; \
			float4 color : COLOR0; \
		}; \
		\
		PS_INPUT main(float3 pos : POSITION, float4 col : COLOR)\
		{ \
			PS_INPUT output;\
			float4 tempPos = float4(pos, 1.0f);\
			tempPos = mul(tempPos, view);\
			tempPos = mul(tempPos, projection);\
			output.pos = tempPos;\
			output.color = col; \
			return output;\
		}";
	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	auto* out = createVertexShader(vertexShader, inLayoutDesc, 2);
	out->SetShaderName("Line");
	return out;
}

auto	D3D11ShaderFactory::CreateUnlitColorShader() -> Shader*
{
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
	auto* out = createPixelShader(pixelShader);
	out->SetShaderName("UNLIT COLOR");
	return out;
}

auto	D3D11ShaderFactory::CreateVertexSkinningShader() -> Shader*
{
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
		cbuffer UnlitColorBuffer : register(b3)\
		{\
			float4	unlitColor;\
		};\
		cbuffer SkelMeshBuffer : register(b2)\
		{\
			uint	boneCount;\
			matrix boneMatrices[99];\
		}\
		\
		struct VS_INPUT \
		{ \
			float3 a_position : POSITION; \
			uint4	a_boneIndices : BLENDINDICES;\
			float4	a_boneWeights : BLENDWEIGHT;\
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

	/*
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
		cbuffer UnlitColorBuffer : register(b3)\
		{\
			float4	unlitColor;\
		};\
		cbuffer SkelMeshBuffer : register(b2)\
		{\
			uint	boneCount;\
			matrix boneMatrices[99];\
		}\
		\
		struct VS_INPUT \
		{ \
			float3 a_position : POSITION; \
			uint4	a_boneIndices : BLENDINDICES;\
			float4	a_boneWeights : BLENDWEIGHT;\
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
			float4 pos = float4(input.a_position, 1.0f);\
			float4 tempPos = float4(0.0f, 0.0f, 0.0f, 1.0f);\
			float lastWeight = 0.0f;\
			lastWeight += input.a_boneWeights.x;\
			tempPos += input.a_boneWeights.x * mul(pos, boneMatrices[input.a_boneIndices.x]);\
			lastWeight += input.a_boneWeights.y;\
			tempPos += input.a_boneWeights.y * mul(pos, boneMatrices[input.a_boneIndices.y]);\
			lastWeight += input.a_boneWeights.z;\
			tempPos += input.a_boneWeights.z * mul(pos, boneMatrices[input.a_boneIndices.z]);\
			lastWeight = 1.0f - lastWeight;\
			tempPos += lastWeight * mul(pos, boneMatrices[input.a_boneIndices.w]);\
			tempPos.w = 1.0f;\
			\
			tempPos = mul(tempPos, model);\
			tempPos = mul(tempPos, view);\
			tempPos = mul(tempPos, projection);\
			output.pos = tempPos;\
			output.color = unlitColor; \
			return output;\
		}";
	*/

	D3D11_INPUT_ELEMENT_DESC inLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	auto* out = createVertexShader(vertexShader, inLayoutDesc, 3);
	out->SetShaderName("Skinning");
	return out;
}

auto	D3D11ShaderFactory::BindShader(Shader const* shader) -> void
{
	if (shader->GetShaderType() == Shader::ShaderType::VERTEX)
	{
		D3D11VertexShader* vtxShader = (D3D11VertexShader*)shader;
		d3d11Context->GetDeviceContext()->VSSetShader(vtxShader->GetShader(), nullptr, 0);
		d3d11Context->GetDeviceContext()->IASetInputLayout(vtxShader->GetInputLayout());
	}
	else if (shader->GetShaderType() == Shader::ShaderType::FRAGMENT)
	{
		D3D11PixelShader* pxShader = (D3D11PixelShader*)shader;
		d3d11Context->GetDeviceContext()->PSSetShader(pxShader->GetShader(), nullptr, 0);
	}
}

auto	D3D11ShaderFactory::DeleteShader(Shader* shader) -> void
{
	if (shader->GetShaderType() == Shader::ShaderType::VERTEX)
	{
		D3D11VertexShader* vtxShader = (D3D11VertexShader*)shader;
		vtxShader->Shutdown();
		DEL(vtxShader);
	}
	else if (shader->GetShaderType() == Shader::ShaderType::FRAGMENT)
	{
		D3D11PixelShader* pxShader = (D3D11PixelShader*)shader;
		pxShader->Shutdown();
		DEL(pxShader);
	}
}

auto	D3D11ShaderFactory::createShader(Shader::ShaderType const& type, MString const& shaderText) -> Shader*
{
	if (type == Shader::ShaderType::VERTEX)
		return createVertexShader(shaderText);
	else if (type == Shader::ShaderType::FRAGMENT)
		return createPixelShader(shaderText);
	else
		return nullptr;
}

auto	D3D11ShaderFactory::createVertexShader(MString const& shaderText) -> Shader*
{
	HRESULT hr = S_OK;

	ID3D10Blob* errorBlob = nullptr;
	ID3D10Blob* shaderBlob = nullptr;
	ID3D11VertexShader*	vtxShader = nullptr;

	D3DCompile(shaderText.Str(), shaderText.Count(), nullptr, nullptr, nullptr
		, "main", VsTargets[FeatureLevelIndex], 0, 0, &shaderBlob, &errorBlob);
	if (!shaderBlob)
	{
		Logger::Log((char*)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}

	d3d11Context->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), 
		shaderBlob->GetBufferSize(), nullptr, &vtxShader);

#if _DEBUG
	d3d11Context->MarkD3D11ObjectName(vtxShader, "Vertex Shader");
#endif

	return NEW D3D11VertexShader(vtxShader, shaderBlob);
}

auto	D3D11ShaderFactory::createVertexShader(MString const& shaderText, D3D11_INPUT_ELEMENT_DESC layoutDesc[], unsigned int layoutCount) -> Shader*
{
	HRESULT hr = S_OK;

	ID3D10Blob* errorBlob = nullptr;
	ID3D10Blob* shaderBlob = nullptr;
	ID3D11VertexShader*	vtxShader = nullptr;
	ID3D11InputLayout* inLayout = nullptr;

	D3DCompile(shaderText.Str(), shaderText.Count(), nullptr, nullptr, nullptr
		, "main", VsTargets[FeatureLevelIndex], 0, 0, &shaderBlob, &errorBlob);
	if (!shaderBlob)
	{
		Logger::Log((char*)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}

	d3d11Context->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), nullptr, &vtxShader);

	d3d11Context->GetDevice()->CreateInputLayout(layoutDesc, layoutCount, shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), &inLayout);

#if _DEBUG
	d3d11Context->MarkD3D11ObjectName(vtxShader, "Vertex Shader");
	d3d11Context->MarkD3D11ObjectName(inLayout, "Vertex Shader input layout");
#endif

	return NEW D3D11VertexShader(vtxShader, shaderBlob, inLayout);
}

auto	D3D11ShaderFactory::createPixelShader(MString const& shaderText) -> Shader*
{
	HRESULT	hr = S_OK;
	ID3D10Blob* shaderBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;
	ID3D11PixelShader*	pxShader = nullptr;

	D3DCompile(shaderText.Str(), shaderText.Count(), nullptr, nullptr, nullptr
		, "main", PsTargets[FeatureLevelIndex], 0, 0, &shaderBlob, &errorBlob);
	if (!shaderBlob)
	{
		Logger::Log((char*)errorBlob->GetBufferPointer());
		hr = E_FAIL;
		errorBlob->Release();
	}
	d3d11Context->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), nullptr, &pxShader);

	SAFE_RELEASE(shaderBlob);

#if _DEBUG
	d3d11Context->MarkD3D11ObjectName(pxShader, "Pixel Shader");
#endif

	return NEW D3D11PixelShader(pxShader);
}