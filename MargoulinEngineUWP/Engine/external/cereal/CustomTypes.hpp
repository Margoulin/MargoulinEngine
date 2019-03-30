#ifndef __CUSTOMTYPESERIALIZATION_HPP__
#define __CUSTOMTYPESERIALIZATION_HPP__

#include <MUtils/Maths/Vector.hpp>

#include <vector>

struct SSubMesh
{
	std::vector<Vector3F>	Vertices;
	std::vector<Vector3F>	Normals;
	std::vector<Vector2F>	UV;
	std::vector<unsigned int>		Indices;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(Vertices, Normals, UV, Indices);
	}
};

template<class Archive>
void serialize(Archive & archive, Vector2F& vec) { archive(vec.x, vec.y); }
template<class Archive>
void serialize(Archive & archive, Vector3F& vec) { archive(vec.x, vec.y, vec.z); }
template<class Archive>
void serialize(Archive & archive, Vector4F& vec) { archive(vec.x, vec.y, vec.z, vec.w); }

#endif /*__CUSTOMTYPESERIALIZATION_HPP__*/