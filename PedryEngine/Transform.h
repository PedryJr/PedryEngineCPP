#pragma once
class Transform : public Component
{

public:

	void Initialize() override;
	void Simulate() override;
	void Update() override;


	void SetRotation(const quat rotation);
	void SetRotation(const float x, const float y, const float z);
	void SetRotation(const vec3 euler);
	void SetRotation(const float x, const float y, const float z, const float w);
	void SetRotation(const float angle, const vec3 axis);

	void AddRotation(const quat rotation);
	void AddRotation(const float x, const float y, const float z);
	void AddRotation(const vec3 euler);
	void AddRotation(const float x, const float y, const float z, const float w);
	void AddRotation(const float angle, const vec3 axis);

	void SetScale(const vec3 scale);
	void SetScale(const float x, const float y, const float z);
	void SetScale(const float scale);

	void AddScale(const vec3 scale);
	void AddScale(const float x, const float y, const float z);
	void AddScale(const float scale);

	void SetPosition(const vec3 position);
	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const float scalar);

	void AddPosition(const vec3 position);
	void AddPosition(const float x, const float y, const float z);
	void AddPosition(const float scalar);

	mat4 scaleMatrix;
	mat4 rotationMatrix;
	mat4 translation;
	mat4 modelMatrix;

	vec3 position;
	vec3 scale;
	quat rotation;

	GLboolean shouldUpdateTransform = false;
	GLuint batchIndex = -1;
	GLuint modelIndex = -1;

	Vector<DrawCallBatch>* outerLocation;

	void SetModelIndex(GLuint modelIndex);
	GLuint GetModelIndex();

	mat4 GetModelMatrix();
	void UpdateModel();

	void SetModelLocation(GLint modelIndex, GLint batchIndex);

};