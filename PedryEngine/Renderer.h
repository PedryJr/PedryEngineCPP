#pragma once
class Renderer : public Component
{

private:

public:
	~Renderer();
	void Initialize() override;
	void Simulate() override;
	void Update() override;
};

