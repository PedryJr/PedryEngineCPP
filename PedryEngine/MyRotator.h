#pragma once

GENERATED_CLASS(MyRotator)

class MyRotator : public Component
{

public:
	float xRot, yRot, zRot;
	float xPos, yPos, zPos;
	float timeLine = timer + offset;

	GLboolean onlyOnce = false;

	vec3 circulationPoint;
	vec3 calculatedPosition;
	vec3 calculatedRotation;
	GLfloat timer;
	GLfloat offset;
	GLfloat radius = 1.f;
	GLfloat heightOffset = 0.f;

	void Initialize();
	void Demolish();
	void __vectorcall Simulate();
	void __vectorcall Update();

	GENERATED_BODY(MyRotator)
};

