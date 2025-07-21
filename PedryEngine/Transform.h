#pragma once

GENERATED_CLASS(Transform);

class Transform : public Component
{

public:
	Transform();
	//Transform(Transform&& other);
	Transform& operator=(const Transform& other);
	void Initialize();
	void Demolish();
	void __vectorcall Simulate();
	void Update();


	void SetRotation(const quat rotation);
	void SetRotation(const GLfloat x, const GLfloat y, const GLfloat z);
	void SetRotation(const vec3 euler);
	void SetRotation(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);
	void SetRotation(const GLfloat angle, const vec3 axis);

	void SetScale(const vec3 scale);
	void SetScale(const GLfloat x, const GLfloat y, const GLfloat z);
	void SetScale(const GLfloat scale);

	void AddScale(const vec3 scale);
	void AddScale(const GLfloat x, const GLfloat y, const GLfloat z);
	void AddScale(const GLfloat scale);

	void SetPosition(const vec3 position);
	void SetPosition(const GLfloat x, const GLfloat y, const GLfloat z);
	void SetPosition(const GLfloat scalar);

	void AddPosition(const vec3 position);
	void AddPosition(const GLfloat x, const GLfloat y, const GLfloat z);
	void AddPosition(const GLfloat scalar);

	mat4 modelMatrix;
	mat4 identityMatrix = glm::identity<mat4>();

	vec3 position;
	vec3 scale;
	quat rotation;

	GLboolean useContigousArray = false;
	GLboolean shouldUpdateTransform = false;
	GLuint batchIndex = -1;
	GLuint modelIndex = -1;

	//Vector<DrawCallBatch>* outerLocation;

	void SetModelLocation(GLint modelIndex, GLint batchIndex);

	Vector<DrawCallBatch>& GetOuterLocation();

	GENERATED_BODY(Transform)
};