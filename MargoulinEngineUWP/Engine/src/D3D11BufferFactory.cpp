#include "D3D11BufferFactory.hpp"
	
#include "D3D11Buffer.hpp"
#include "CoreMinimal.hpp"

#include "D3D11Context.hpp"
#include "SubMeshData.hpp"

auto	D3D11BufferFactory::SetContext(Context* value) -> void
{ 
	context = value; 
	d3d11Context = (D3D11Context*)context;
}

auto	D3D11BufferFactory::GenerateVertexBuffer(SubMeshData* data, bool dynamic) -> GPUBuffer*
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = data->GetVertices();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(data->GetVerticesDataSize(), D3D11_BIND_VERTEX_BUFFER);
	if (dynamic)
	{
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}
	d3d11Context->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &buffer);

	D3D11Buffer* outBuffer = NEW D3D11Buffer(buffer);
	outBuffer->SetBufferType(BufferType::VERTEX);
	outBuffer->SetStride(sizeof(Vector3F));
	return outBuffer;
}

auto	D3D11BufferFactory::GenerateIndexBuffer(SubMeshData* data) -> GPUBuffer*
{
	ID3D11Buffer* buffer = nullptr;
	
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = data->GetIndices();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(data->GetIndicesCount() * sizeof(unsigned int), D3D11_BIND_INDEX_BUFFER);
	d3d11Context->GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &buffer);

	D3D11Buffer* outBuffer = NEW D3D11Buffer(buffer);
	outBuffer->SetBufferType(BufferType::INDEX);
	return outBuffer;
}
	
auto	D3D11BufferFactory::GenerateBuffer(unsigned int size) -> GPUBuffer*
{
	ID3D11Buffer* buffer = nullptr;
	
	CD3D11_BUFFER_DESC bufferDesc(size, D3D11_BIND_CONSTANT_BUFFER);
	d3d11Context->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &buffer);

	D3D11Buffer* outBuffer = NEW D3D11Buffer(buffer);
	outBuffer->SetBufferType(BufferType::GENERAL);
	return outBuffer;
}

auto	D3D11BufferFactory::DeleteBuffer(GPUBuffer* buffer) -> void
{
	D3D11Buffer* d3d11Buffer = (D3D11Buffer*)buffer;
	d3d11Buffer->Shutdown();
	DEL(buffer);
}