#include "D3D11BufferFactory.hpp"
	
#include "D3D11Buffer.hpp"
#include "CoreMinimal.hpp"

#include "D3D11Context.hpp"
#include "SubMeshData.hpp"

#include "SkeletalMesh.hpp"

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

auto	D3D11BufferFactory::GenerateVertexBuffer(SkeletalMesh* data, bool dynamic) -> GPUBuffer*
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
	
auto	D3D11BufferFactory::GenerateVertexBuffer(unsigned int bufferSize, unsigned int strideValue, void* adress, bool dynamic) -> GPUBuffer*
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = adress;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(bufferSize, D3D11_BIND_VERTEX_BUFFER);
	if (dynamic)
	{
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}
	d3d11Context->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &buffer);

	D3D11Buffer* outBuffer = NEW D3D11Buffer(buffer);
	outBuffer->SetBufferType(BufferType::VERTEX);
	outBuffer->SetStride(strideValue);
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

auto	D3D11BufferFactory::GenerateIndexBuffer(SkeletalMesh* data) -> GPUBuffer*
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
	
auto	D3D11BufferFactory::GenerateVertexBufferArray(unsigned int bufferCount, SkeletalMesh* mesh) -> GPUBufferArray*
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = mesh->GetVertices();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(mesh->GetVerticesDataSize(), D3D11_BIND_VERTEX_BUFFER);
	d3d11Context->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &buffer);

	D3D11BufferArray* outBufferArray = NEW D3D11BufferArray(2);
	outBufferArray->SetStride(0, sizeof(Vector3F));
	outBufferArray->SetBuffer(0, buffer);
	outBufferArray->SetBufferType(BufferType::VERTEX);

	ID3D11Buffer* buffer2 = nullptr;
	D3D11_SUBRESOURCE_DATA vertexBufferData2 = { 0 };
	vertexBufferData2.pSysMem = mesh->GetSkins();
	vertexBufferData2.SysMemPitch = 0;
	vertexBufferData2.SysMemSlicePitch = 0;
	int t = sizeof(float) * 4 + sizeof(unsigned int) * 4 * mesh->GetVerticesCount();
	CD3D11_BUFFER_DESC vertexBufferDesc2(t, D3D11_BIND_VERTEX_BUFFER);
	d3d11Context->GetDevice()->CreateBuffer(&vertexBufferDesc2, &vertexBufferData2, &buffer2);

	outBufferArray->SetStride(1, sizeof(float) * 4 + sizeof(unsigned int) * 4);
	outBufferArray->SetBuffer(1, buffer2);
	outBufferArray->SetBufferCount(2);
	outBufferArray->SetInitializedBufferCount(2);

	return outBufferArray;
}

auto	D3D11BufferFactory::DeleteBuffer(GPUBuffer* buffer) -> void
{
	D3D11Buffer* d3d11Buffer = (D3D11Buffer*)buffer;
	d3d11Buffer->Shutdown();
	DEL(buffer);
}