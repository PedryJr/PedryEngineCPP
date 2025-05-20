#pragma once

#include "PedryEngine.h"

GameObject::GameObject()
{

	transform = AddComponent<Transform>();

}

GameObject::~GameObject()
{
	// Clean up components
	for (Component* component : components) delete component;
	components.clear();
	delete transform;
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
	componentCount++;
	component->transform = transform;
	component->gameObject = this;
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
		componentCount--;
	}
}

void GameObject::Update()
{
	for (int i = 0; i < componentCount; i++) components[i]->Update();
}
void GameObject::Simulate()
{
	for (int i = 0; i < componentCount; i++) components[i]->Simulate();
}