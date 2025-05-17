#include "PedryEngine.h"

void MyRotator::Initialize()
{
	offset = 0;
	transform->SetScale(vec3(1.5f));
}

void MyRotator::Simulate()
{

	timer += DeltaTime;

	timeLine = timer + (2.f * glm::pi<float>() / 8.f * offset);

	xPos = glm::sin(timeLine) * 15.f;
	zPos = glm::cos(timeLine) * 15.f;
	xRot = xPos * 360.f / 15.f;
	zRot = zPos * 360.f / 15.f;

}

void MyRotator::Update()
{

	transform->SetRotation(vec3(xRot, 0, zRot));
	transform->SetPosition(vec3(xPos, -20.f, zPos));

}
