#pragma once
class DrawCallBatch
{

public:
	DrawCallBatch(Shader* shader, Mesh* mesh);
	~DrawCallBatch();
	void AddBatchInstance(Transform& instanceSource);

	Vector<Transform> corespondingTransforms;
	Vector<mat4> modelMatrices;
	Shader* shader;
	Mesh* mesh;

	bool notifyLayoutChange;

};

