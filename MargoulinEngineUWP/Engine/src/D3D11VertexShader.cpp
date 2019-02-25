#include "D3D11VertexShader.hpp"

D3D11VertexShader::D3D11VertexShader(ID3D11VertexShader* shader, ID3D10Blob* shadBlob, ID3D11InputLayout* inputLayout)
	:vtxShader(shader), shaderBlob(shadBlob), inLayout(inputLayout)
{}
D3D11VertexShader::D3D11VertexShader(ID3D11VertexShader* shader, ID3D10Blob* shadBlob)
	:vtxShader(shader), shaderBlob(shadBlob)
{}

auto	D3D11VertexShader::Shutdown() -> void 
{
	*inLayout.ReleaseAndGetAddressOf() = nullptr;
	*vtxShader.ReleaseAndGetAddressOf() = nullptr; 
}