#pragma once
class GameObject
{

public:

	GameObject();
	~GameObject();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
	void Update();

	template<typename T>
	T* GetComponent()
	{
		for (Component* component : components)
		{
			if (T* castedComponent = dynamic_cast<T*>(component))
			{
				return castedComponent;
			}
		}
		return nullptr;
	}

	template<typename T>
	T* AddComponent()
	{
		T* component = new T();
		AddComponent(component);
		return component;
	}

	Vector<Component*> components;

};