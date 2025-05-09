#pragma once
class Transform;
class DrawCallBatch
{

public:
	DrawCallBatch(Shader* shader, Mesh* mesh);
	~DrawCallBatch();
	void AddBatchInstance(Transform* instanceSource);

	Vector<mat4> modelMatrices;
	Shader* shader;
	Mesh* mesh;

};

