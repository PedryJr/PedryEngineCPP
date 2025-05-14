#include "PedryEngine.h"

void MyRotator::Initialize()
{
	offset = 0;
}

void MyRotator::Simulate()
{
}

void MyRotator::Update()
{

	timer += DeltaTime / 3;

	float xRot, zRot;
	float xPos, zPos;
	float timeLine = timer + offset;

	xRot = glm::sin(timeLine) * 360;
	zRot = glm::cos(timeLine) * 360;
	xPos = glm::sin(timeLine) * 5 * glm::sin(timer);
	zPos = glm::cos(timeLine) * 5 * glm::sin(timer);

	transform->SetRotation(vec3(xRot, 0, zRot));
	transform->SetPosition(vec3(xPos, 0, zPos));

}
