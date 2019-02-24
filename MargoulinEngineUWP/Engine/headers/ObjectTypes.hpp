#ifndef __OBJECTTYPES_HPP__
#define __OBJECTTYPES_HPP__

#include "String.hpp"

enum ObjectType
{
	OBJECT,
	NODE,
	COMPONENT,
	GRAPHIC_COMPONENT,
	MESH_COMPONENT,
	SKELETAL_MESH_COMPONENT,
	RENDERER_2D_COMPONENT,
	CAMERA_COMPONENT
};

static MString ObjectTypeNames[] = { "Node", "Component", "Graphic Component", "Mesh Component", "Renderer 2D Component", "Camera Component" };

#endif /*__OBJECTTYPES_HPP__*/