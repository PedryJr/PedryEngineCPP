#include "PedryEngine.h"

void MyRotator::Initialize()
{
	offset = 0;
	GetGameObject().GetComponent<Transform>().SetScale(vec3(1.5f));
}

void __vectorcall MyRotator::Simulate()
{

	timer += DeltaTime / 5;

	timeLine = timer + (2.f * glm::pi<float>() / 8.f * offset);

	xPos = glm::sin(timeLine) * 15.f * radius;
	yPos = -30.f + heightOffset;
	zPos = glm::cos(timeLine) * 15.f * radius;

	xRot = xPos * 360.f / 15.f;
	zRot = zPos * 360.f / 15.f;
	yRot = yPos * 360.f / 15.f;

	calculatedPosition = vec3(xPos, yPos, zPos) + circulationPoint;
	calculatedRotation = vec3(xRot, yRot, zRot);

}

void __vectorcall MyRotator::Update()
{
	Transform& transform = GetGameObject().GetComponent<Transform>();
	transform.SetPosition(calculatedPosition);
	transform.SetRotation(calculatedRotation);
}

void MyRotator::Demolish()
{

}
GENERATED_ACTION(MyRotator)