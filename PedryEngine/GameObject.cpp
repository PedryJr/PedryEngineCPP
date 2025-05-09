#pragma once
#include "PedryEngine.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	// Clean up components
	for (Component* component : components) delete component;
	components.clear();
}

void GameObject::AddComponent(Component* component)
{

	if (component == nullptr) return;
	// Check if the component is already added
	for (Component* existingComponent : components)
	{
		if (existingComponent == component) return;
	}
	components.push_back(component);
	component->Initialize();
}

void GameObject::RemoveComponent(Component* component)
{
	if (component == nullptr) return;
	// Find and remove the component
	auto it = std::remove(components.begin(), components.end(), component);
	if (it != components.end())
	{
		component->~Component();
		components.erase(it, components.end());
	}
}

void GameObject::Update()
{
	for (Component* component : components) component->Update();
}