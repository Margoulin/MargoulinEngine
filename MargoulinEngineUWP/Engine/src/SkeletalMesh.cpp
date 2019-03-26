#include "SkeletalMesh.hpp"

Rig::Rig(unsigned int size)
{
	boneCount = size;
	bones = NEW Bone[size];
}

Rig::~Rig()
{
	DELARRAY(bones);
	boneCount = 0;
}

void	SkeletalMesh::Shutdown()
{

}