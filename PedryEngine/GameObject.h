#pragma once
class GameObject
{

public:

	Transform* transform;

	GameObject();
	~GameObject();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
	void Update();
	void Simulate();

	template<typename T>
	requires std::is_base_of_v<Component, T>
	T* GetComponent()
	{
		for (Component* component : components)
		{
			if (typeid(*component) == typeid(T))
			{
				return component;
			}
		}
		return nullptr;
	}

	template<typename T>
	requires std::is_base_of_v<Component, T>
	T* AddComponent()
	{
		T* component = new T();
		AddComponent(component);
		return component;
	}

private:
	
	GLuint componentCount = 0;
	Vector<Component*> components;

};