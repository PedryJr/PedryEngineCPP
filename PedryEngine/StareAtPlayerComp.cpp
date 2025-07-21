#include "PedryEngine.h"

void StareAtPlayerComp::Initialize()
{

}

void StareAtPlayerComp::Simulate()
{

}

void StareAtPlayerComp::Update()
{

	//Arm transfrom
	Transform& transform = GetGameObject().GetComponent<Transform>();

	//Claw position
	vec3 clawPos = GlobalCamera::aimPositionTarget;

	//Arm position
	vec3 armTipPos = transform.position;

	//Direction from arm to claw
	vec3 direction = glm::normalize(clawPos - armTipPos);

	//Rotation for arm to reach claw
	quat lookRotation = glm::quatLookAt(direction, vec_UP);

	//quat bruh = glm::rotate<GLfloat>(glm::identity<quat>(), glm::radians(20), vec3(1, 0, 0));

	//Apply quaternion as rotation
	transform.SetRotation(glm::slerp<GLfloat>(transform.rotation, lookRotation, DeltaTime * 6.f));

	transform.SetScale(5.f);

}

void StareAtPlayerComp::Demolish()
{

}
GENERATED_ACTION(StareAtPlayerComp)