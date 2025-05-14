#pragma once

class Component
{
public:
	virtual void Initialize() {}
	virtual void Simulate() {}
	virtual void Update() {}
	GameObject* gameObject;
	Transform* transform;
private:
};

