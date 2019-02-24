#ifndef __OBJECTHANDLE_HPP__
#define __OBJECTHANDLE_HPP__

#include <memory>
#include "Object.hpp"
#include "Logger.hpp"

template<class ObjectType>
class ObjectStrongPtr
{
public:
	ObjectStrongPtr(std::shared_ptr<Object> basicShared) : strongPointer(basicShared)
	{
		objectPointer = dynamic_cast<ObjectType*>(strongPointer.get());
	}

	ObjectStrongPtr(const ObjectStrongPtr&) = delete;
	ObjectStrongPtr(ObjectStrongPtr&& other)
	{
		strongPointer = std::move(other.strongPointer);
		objectPointer = other.objectPointer;
	}

	~ObjectStrongPtr() = default;

	auto	operator=(const ObjectStrongPtr&)->ObjectStrongPtr& = delete;
	auto	operator=(ObjectStrongPtr&& other)->ObjectStrongPtr&
	{
		strongPointer = std::move(other.strongPointer);
		return *this;
	}

protected:

private:
	std::shared_ptr<Object>	strongPointer;
	ObjectType*				objectPointer = nullptr;
};

template<class ObjectType>

class ObjectWeakPtr
{
public:
	ObjectWeakPtr(std::shared_ptr<Object> basicShared) :weakPointer(basicShared){}
	ObjectWeakPtr(const ObjectWeakPtr&) = delete;
	ObjectWeakPtr(ObjectWeakPtr&& other)
	{
		weakPointer = std::move(other.weakPointer);
	}

	~ObjectWeakPtr() = default;

	auto	IsValid() const -> bool { return !weakPointer.expired(); }
	auto	GetRawObjectPtr() const -> Object* { return weakPointer.lock().get(); }
	auto	GetObjectPtr() const -> ObjectType* { return dynamic_cast<ObjectType*>(weakPointer.lock().get()); }

	auto	operator=(const ObjectWeakPtr&)->ObjectWeakPtr& = delete;
	auto	operator=(ObjectWeakPtr&&)->ObjectWeakPtr&
	{
		weakPointer = std::move(other.weakPointer);
	}

protected:

private:
	std::weak_ptr<Object>	weakPointer;

};


class ObjectHandle 
{
public:
	ObjectHandle(Object* obj) : objectPointer(obj) {}
	ObjectHandle(const ObjectHandle&) = delete;
	ObjectHandle(ObjectHandle&&) = delete;
	~ObjectHandle() { if (objectPointer.use_count() != 1) Logger::Log("Object handle destructor : the shared ptr doesn't have 1 ref count"); }

	template<typename T> 
	auto	CreateObjectStrongPtr() -> ObjectStrongPtr<T> { return ObjectStrongPtr<T>(objectPointer); }
	template<typename T> 
	auto	CreateObjectWeakPtr() -> ObjectWeakPtr<T> { return ObjectWeakPtr<T>(objectPointer); }

	auto	CanBeDeleted() const -> bool { return objectPointer.use_count() == 1; }

	auto	operator=(const ObjectHandle&)->ObjectHandle& = delete;
	auto	operator=(ObjectHandle&&)->ObjectHandle& = delete;

protected:

private:
	std::shared_ptr<Object>	objectPointer;
};


#endif /*__OBJECTHANDLE_HPP__*/