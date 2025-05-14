#pragma once
class Renderer : public Component
{
public:
	void Initialize() override;
	void Simulate() override;
	void Update() override;
};

