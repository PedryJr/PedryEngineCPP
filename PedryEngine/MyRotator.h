#pragma once
class MyRotator : public Component
{

	float xRot, zRot;
	float xPos, zPos;
	float timeLine = timer + offset;

	GLfloat timer;

public:

	GLfloat offset;

	void Initialize() override;
	void Simulate() override;
	void Update() override;

};

