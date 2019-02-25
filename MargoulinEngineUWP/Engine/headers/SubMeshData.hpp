#ifndef __SUBMESHDATA_HPP__
#define __SUBMESHDATA_HPP__

#include <vector>
#include "DirectXHelper.hpp"
#include <d3d11.h>
#include <wrl/client.h>

#include "cereal/CustomTypes.hpp"

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
		CD3D11_BUFFER_DESC vertexBufferDesc((unsigned int)vertices.size() * (unsigned int)(sizeof(Vector3F)), D3D11_BIND_VERTEX_BUFFER);
		device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, g_VertexBuffer.GetAddressOf());

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = &indices[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc((unsigned int)sizeof(unsigned int)* (unsigned int)indices.size(), D3D11_BIND_INDEX_BUFFER);
		device->CreateBuffer(&indexBufferDesc, &indexBufferData, g_IndexBuffer.GetAddressOf());
		inVRAM = true;
	}

	auto	Shutdown() -> void { *g_IndexBuffer.ReleaseAndGetAddressOf() = nullptr;  *g_VertexBuffer.ReleaseAndGetAddressOf() = nullptr; }

	auto	AddVertice(Vector3F val) -> void { vertices.push_back(val); }
	auto	AddNormal(Vector3F val) -> void { normals.push_back(val); }
	auto	AddTexCoords(Vector2F val) -> void { uv.push_back(val); }
	auto	AddIndice(unsigned int const& val) -> void { indices.push_back(val); }

	auto	IsInVRAM() const -> bool const { return inVRAM; }

	auto	GetIndexBuffer() const -> ID3D11Buffer* { return g_IndexBuffer.Get(); }
	auto	GetVertexBuffer() const-> ID3D11Buffer* const* { return g_VertexBuffer.GetAddressOf(); }

	//auto	GetVerticesDataSize() const -> unsigned int { return vertices.size() * sizeof(Vector3F) + normals.size() * sizeof(Vector3F) + uv.size() * sizeof(Vector2F); }
	auto	GetVerticesDataSize() const -> unsigned int { return vertices.size() * sizeof(Vector3F); }
	auto	GetVerticesCount() const -> unsigned int const { return (unsigned int)vertices.size(); }
	auto	GetNormalsCount() const -> unsigned int const { return (unsigned int)normals.size(); }
	auto	GetIndicesCount() const -> unsigned int const { return (unsigned int)indices.size(); }
	auto	GetVertices() -> Vector3F* { return &vertices[0]; }
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
	std::vector<Vector3F>		vertices;
	std::vector<Vector3F>		normals;
	std::vector<Vector2F>		uv;
	std::vector<unsigned int>	indices;

	bool inVRAM = false;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	g_VertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	g_IndexBuffer = nullptr;
};


#endif /*__SUBMESHDATA_HPP__*/