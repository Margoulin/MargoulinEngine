#include "TextureLoader.hpp"

#include "ResourcesManager.hpp"
#include "Engine.hpp"
#include "TextureResource.hpp"

#ifndef VITA
#include "DirectXTex.h"
#include "D3D11TextureData.hpp"
#endif

MString	TextureLoader::LocalFolderName = ".";

auto	TextureLoader::LoadTextureFromFile(MString const& filename, bool const absolutePath) -> bool
{
#ifndef VITA
	DirectX::ScratchImage*	image = NEW DirectX::ScratchImage();

	MWString temp = MWString::FromString(LocalFolderName + "/" + filename);
	HRESULT hr = DirectX::LoadFromWICFile(temp.Str(), DirectX::WIC_FLAGS_NONE, nullptr, *image);
	if (FAILED(hr))
	{
		DEL(image);
		return false;
	}

	ResourcesManager*	rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int texId = rsMgr->CreateResource<TextureResource>();
	TextureResource* textureRes = rsMgr->GetResource<TextureResource>(texId);
	textureRes->SetName(filename.Str());
	D3D11TextureData* texData = NEW D3D11TextureData();
	texData->SetImageData(image);
	textureRes->SetTextureData(texData);
	return true;
#else
	return false;
#endif
}