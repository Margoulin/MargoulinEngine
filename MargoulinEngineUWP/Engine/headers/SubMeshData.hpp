#ifndef __SUBMESHDATA_HPP__
#define __SUBMESHDATA_HPP__

#include <vector>
#include <DirectXMath.h>
#include "DirectXHelper.hpp"
#include <d3d11.h>

struct SSubMesh
{
	std::vector<DirectX::XMFLOAT3>	Vertices;
	std::vector<DirectX::XMFLOAT3>	Normals;
	std::vector<DirectX::XMFLOAT2>	UV;
	std::vector<unsigned int>		Indices;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(Vertices, Normals, UV, Indices);
	}
};

class SubMeshData
{
public:
	SubMeshData() = default;
	SubMeshData(const SubMeshData&) = delete;
	SubMeshData(SubMeshData&&) = delete;
	~SubMeshData() = default;

	auto	UploadInVRAM(ID3D11Device* device) -> void
	{
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = &vertices[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc((unsigned int)vertices.size() * (unsigned int)(sizeof(DirectX::XMFLOAT3)), D3D11_BIND_VERTEX_BUFFER);
		device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &g_VertexBuffer);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = &indices[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc((unsigned int)sizeof(unsigned int)* (unsigned int)indices.size(), D3D11_BIND_INDEX_BUFFER);
		device->CreateBuffer(&indexBufferDesc, &indexBufferData, &g_IndexBuffer
		);
		inVRAM = true;
	}

	auto	Shutdown() -> void { SAFE_RELEASE(g_IndexBuffer);  SAFE_RELEASE(g_VertexBuffer); }

	auto	AddVertice(DirectX::XMFLOAT3 val) -> void { vertices.push_back(val); }
	auto	AddNormal(DirectX::XMFLOAT3 val) -> void { normals.push_back(val); }
	auto	AddTexCoords(DirectX::XMFLOAT2 val) -> void { uv.push_back(val); }
	auto	AddIndice(unsigned int const& val) -> void { indices.push_back(val); }

	auto	IsInVRAM() const -> bool const { return inVRAM; }

	auto	GetIndexBuffer() const -> ID3D11Buffer* { return g_IndexBuffer; }
	auto	GetVertexBuffer() const-> ID3D11Buffer* const* { return &g_VertexBuffer; }

	auto	GetVerticesCount() const -> unsigned int const { return (unsigned int)vertices.size(); }
	auto	GetNormalsCount() const -> unsigned int const { return (unsigned int)normals.size(); }
	auto	GetIndicesCount() const -> unsigned int const { return (unsigned int)indices.size(); }
	auto	GetVertices() -> DirectX::XMFLOAT3* { return &vertices[0]; }
	auto	GetIndices() -> unsigned int* { return &indices[0]; }
	auto	GetSerializeData() const -> SSubMesh
	{
		SSubMesh	ret;
		ret.Vertices = vertices;
		ret.Normals = normals;
		ret.UV = uv;
		ret.Indices = indices;
		return ret;
	}

	auto	operator = (const SubMeshData&)->SubMeshData& = delete;
	auto	operator = (SubMeshData&&)->SubMeshData& = delete;

protected:

private:
	std::vector<DirectX::XMFLOAT3>	vertices;
	std::vector<DirectX::XMFLOAT3>	normals;
	std::vector<DirectX::XMFLOAT2>	uv;
	std::vector<unsigned int>		indices;

	bool inVRAM = false;
	ID3D11Buffer*	g_VertexBuffer = nullptr;
	ID3D11Buffer*	g_IndexBuffer = nullptr;
};


#endif /*__SUBMESHDATA_HPP__*/