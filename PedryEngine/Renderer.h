#pragma once

GENERATED_CLASS(Renderer)

class Renderer : public Component
{


private:

	bool initialized = false;
	Shader* shader;
	Mesh* sphere;

	GLuint batchIndex;
	GLuint modelIndex;
	DrawCallBatch* batches;
	mat4* modelMatrices;

public:
	void Initialize();
	void Demolish();
	void Simulate();
	void Update();

	void ChangeMesh(Mesh* newMesh);
	void ChangeTexture(GLuint64 texture);


	GENERATED_BODY(Renderer)
};

