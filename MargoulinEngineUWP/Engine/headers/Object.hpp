#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "ObjectTypes.hpp"
#include "CoreMinimal.hpp"

class Object
{
public:
	Object() = default;
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	~Object() = default;

	virtual auto	Initialize() -> void = 0;
	virtual auto	Shutdown() -> void = 0;

	virtual auto	Update() -> void = 0;

	auto	SetID(unsigned int const& value) -> void { ID = value; }

	auto	GetID() const -> unsigned int { return ID; }

	//virtual auto	CanBeGarbageCollected() const -> bool = 0;
	virtual auto	GetObjectType() const -> ObjectType const { return ObjectType::OBJECT; }
	virtual auto	GetObjectTypeName() const -> MString const { return ObjectTypeNames[ObjectType::OBJECT]; }
	auto	operator = (const Object&)->Object& = delete;
	auto	operator = (Object&&)->Object& = delete;

protected:	
	unsigned int	ID = 0;

private:
#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void {}
#endif
};


#endif /*__OBJECT_HPP__*/