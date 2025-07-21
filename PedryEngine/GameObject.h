#pragma once
class GameObject
{

public:

	//Transform* transform;

	GameObject();
	GameObject(GLuint newId);

	template<typename T>
	requires std::is_base_of_v<Component, T>
	FORCE_INLINE T& GetComponent()
	{

		return T::componentArray[components[T::type]];
	}

	std::unordered_map<GLuint, GLuint> components;

	template<typename T>
	requires std::is_base_of_v<Component, T>
	FORCE_INLINE T* AddComponent()
	{
		components[T::type] = T::componentArray.size();
		T::CompInit();
		T::componentArray[components[T::type]].owner = ID;
		T::componentArray[components[T::type]].Initialize();
		return &T::componentArray[components[T::type]];
	}


	static void UpdateAll();
	GLuint ID;

	static Vector<GameObject> objArr;
	static GameObjectHandle CreateGameObject();

private:

};