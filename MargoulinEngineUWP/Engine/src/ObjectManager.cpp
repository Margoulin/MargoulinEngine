#include "ObjectManager.hpp"

#include "Object.hpp"
#include "MemoryMacro.hpp"

auto	ObjectManager::Shutdown() -> void
{
	for (auto& obj : objects)
	{
		obj->Shutdown();
		DEL(obj);
	}
}

auto	ObjectManager::Update() -> void
{
	GarbageCollection();
}

auto	ObjectManager::GarbageCollection() -> void
{
	for (auto& obj : objectsToDelete)
	{
		if (obj)
			removeObject(obj);
	}
	objectsToDelete.clear();
}

auto	ObjectManager::AddRelation(Object* obj, Object* related) -> void
{
	relationTable[obj].push_back(related);
	relationTable[related].push_back(obj);
}

auto	ObjectManager::RemoveRelation(Object* first, Object* second) -> void
{
	for (auto it = relationTable[first].begin(); 
		it != relationTable[first].end(); ++it)
	{
		if ((*it) == second)
		{
			relationTable[first].erase(it);
			break;
		}
	}
	for (auto it = relationTable[second].begin(); 
		it != relationTable[second].end(); ++it)
	{
		if ((*it) == first)
		{
			relationTable[second].erase(it);
			break;
		}
	}
}

auto	ObjectManager::GetObjectsOfType(ObjectType const type) const -> std::vector<Object*>
{
	std::vector<Object*>	ret;
	auto it = typeTableObjects.find(type);

	if (it != typeTableObjects.end())
		ret = it->second;
	return ret;
}

auto	ObjectManager::addObjectToDatabase(Object* objToAdd) -> void
{
	ObjectType	type = objToAdd->GetObjectType();
	auto	typeTableIt = typeTableObjects.find(type);
	if (typeTableIt != typeTableObjects.end())
		typeTableIt->second.push_back(objToAdd);
	else
	{
		std::vector<Object*>	tempVec;
		tempVec.push_back(objToAdd);
		typeTableObjects.emplace(type, tempVec);
	}
	relationTable.emplace(objToAdd, std::vector<Object*>());
}

auto	ObjectManager::removeObject(Object* obj) -> void
{
	for (Object* relatedObject : relationTable[obj])
	{
		auto it = relationTable[relatedObject].begin();
		for (it; it != relationTable[relatedObject].end(); ++it)
		{
			if (*it == obj)
			{
				relationTable[relatedObject].erase(it);
				break;
			}
		}
	}
	for (auto it = typeTableObjects[obj->GetObjectType()].begin();
		it != typeTableObjects[obj->GetObjectType()].end(); ++it)
	{
		if ((*it) == obj)
		{
			typeTableObjects[obj->GetObjectType()].erase(it);
			break;
		}
	}
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it) == obj)
		{
			DEL(obj);
			objects.erase(it);
			break;
		}
	}
}

#include <imgui/imgui.h>
#ifdef _DEBUG

auto	ObjectManager::ImGuiUpdate() -> void
{
	for (auto& obj : objects)
	{
		ImGui::Value("ID", obj->GetID());
		MString temp = "Type : " + obj->GetObjectTypeName();
		ImGui::Text(temp.Str());
	}
}

#endif