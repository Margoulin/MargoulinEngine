#ifndef __SUBMESHDATA_HPP__
#define __SUBMESHDATA_HPP__

#include "MemoryMacro.hpp"

#include <vector>
#include "GPUBuffer.hpp"

#include "cereal/CustomTypes.hpp"
#include "D3D11Buffer.hpp"

class SubMeshData
{
public:
	SubMeshData() = default;
	SubMeshData(const SubMeshData&) = delete;
	SubMeshData(SubMeshData&&) = delete;
	~SubMeshData() = default;

	auto	Shutdown() -> void 
	{
		((D3D11Buffer*)indexBuffer)->Shutdown();
		DEL(indexBuffer);
		((D3D11Buffer*)vertexBuffer)->Shutdown();
		DEL(vertexBuffer);
	}

	auto	AddVertice(Vector3F val) -> void { vertices.push_back(val); }
	auto	AddNormal(Vector3F val) -> void { normals.push_back(val); }
	auto	AddTexCoords(Vector2F val) -> void { uv.push_back(val); }
	auto	AddIndice(unsigned int const& val) -> void { indices.push_back(val); }

	auto	IsInVRAM() const -> bool const { return inVRAM; }

	auto	SetVertice(unsigned int const& id, Vector3F const& value) -> void { vertices[id] = value; }
	auto	SetUV(unsigned int const& id, Vector2F const& value) -> void { uv[id] = value; }
	auto	SetVertexBuffer(GPUBuffer* buffer) -> void { vertexBuffer = buffer; }
	auto	SetIndexBuffer(GPUBuffer* buffer) -> void { indexBuffer = buffer; }
	auto	SetInVRAM(bool val) -> void { inVRAM = val; }

	auto	GetIndexBuffer() const -> GPUBuffer* { return indexBuffer; }
	auto	GetVertexBuffer() const-> GPUBuffer* { return vertexBuffer; }

	//auto	GetVerticesDataSize() const -> unsigned int { return vertices.size() * sizeof(Vector3F) + normals.size() * sizeof(Vector3F) + uv.size() * sizeof(Vector2F); }
	auto	GetVerticesDataSize() const -> unsigned int { return vertices.size() * sizeof(Vector3F) + uv.size() * sizeof(Vector2F); }
	auto	GetVerticesCount() const -> unsigned int const { return (unsigned int)vertices.size(); }
	auto	GetNormalsCount() const -> unsigned int const { return (unsigned int)normals.size(); }
	auto	GetIndicesCount() const -> unsigned int const { return (unsigned int)indices.size(); }
	auto	GetUVCount() const -> unsigned int const { return (unsigned int)uv.size(); }
	auto	GetVertices() -> Vector3F* { return &vertices[0]; }
	auto	GetIndices() -> unsigned int* { return &indices[0]; }
	auto	GetUV() -> Vector2F* { return &uv[0]; }
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
	GPUBuffer*								vertexBuffer = nullptr;
	GPUBuffer*								indexBuffer = nullptr;
};


#endif /*__SUBMESHDATA_HPP__*/