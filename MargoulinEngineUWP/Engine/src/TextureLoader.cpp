#include "TextureLoader.hpp"

#include "DirectXTex.h"
#include "ResourcesManager.hpp"
#include "Engine.hpp"
#include "TextureResource.hpp"

#include <wrl/wrappers/corewrappers.h>

MString	TextureLoader::LocalFolderName = ".";

auto	TextureLoader::LoadTextureFromFile(MString const& filename, bool const absolutePath) -> bool
{
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
	textureRes->SetImageData(image);
	return true;
}