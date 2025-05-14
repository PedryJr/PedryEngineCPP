#pragma once
class Transform : public Component
{

public:

	void Initialize() override;
	void Simulate() override;
	void Update() override;

	void SetPosition(vec3 position);
	void SetScale(vec3 scale);
	void SetRotation(vec3 euler);

	vec3 position;
	vec3 scale;
	quat rotation;

	GLuint modelIndex;
	DrawCallBatch* assignedBatch = nullptr;

	void SetModelIndex(GLuint modelIndex);
	GLuint GetModelIndex();

	mat4 GetModelMatrix();
	void UpdateModel();

	void SetModelLocation(DrawCallBatch& assignedBatch, GLint index);

};