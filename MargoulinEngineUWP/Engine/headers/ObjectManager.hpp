#ifndef __OBJECTMANAGER_HPP__
#define __OBJECTMANAGER_HPP__

#include "Service.hpp"

#include <vector>
#include <map>
#include "ObjectTypes.hpp"

class Object;

class ObjectManager : public Service
{
public:
	ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager(ObjectManager&&) = delete;
	~ObjectManager() = default;

	enum RelationType
	{
		CHILD,
		COMPONENT
	};

	virtual auto	Initialize() -> void {}
	virtual auto	Shutdown() -> void;

	auto	Update() -> void;

	template<typename T>
	T*	Create()
	{
		T*	obj = new T();
		Object* baseObj = (Object*)obj;
		baseObj->SetID(idTotal);
		idTotal++;

		baseObj->Initialize();
		objects.push_back(baseObj);
		addObjectToDatabase(baseObj);

		return obj;
	}

	auto	DeleteObject(Object* obj) -> void { objectsToDelete.push_back(obj); }
	auto	GarbageCollection() -> void;

	auto	AddRelation(Object* obj, Object* related) -> void;
	auto	RemoveRelation(Object* first, Object* second) -> void;

	auto	GetObject(unsigned int const& value) const -> Object* { return objects[value]; }
	template<typename T>
	auto	GetObject(unsigned int const& value) const -> T* { return (T*)objects[value]; }
	auto	GetObjectsOfType(ObjectType const type) const -> std::vector<Object*>;

	auto	operator = (const ObjectManager&)->ObjectManager& = delete;
	auto	operator = (ObjectManager&&)->ObjectManager& = delete;

protected:

private:
	auto	addObjectToDatabase(Object* obj) -> void;
	auto	removeObject(Object* obj) -> void;

	std::vector<Object*>						objects;
	std::map<ObjectType, std::vector<Object*>>	typeTableObjects;
	std::map<Object*, std::vector<Object*>>		relationTable;
	std::vector<Object*>						objectsToDelete;

	unsigned int			idTotal = 0;

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void;
#endif
};


#endif /*__OBJECTMANAGER_HPP__*/