#pragma once
class MyRotator : public Component
{

	GLfloat timer;

public:

	GLfloat offset;

	void Initialize() override;
	void Simulate() override;
	void Update() override;

};

