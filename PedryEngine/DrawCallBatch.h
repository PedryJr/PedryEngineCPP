#pragma once
class DrawCallBatch
{

public:
	DrawCallBatch(Shader* shader, Mesh* mesh);
	~DrawCallBatch();
	void AddBatchInstance(Transform& instanceSource, GLint batchIndex);

	Vector<Transform> corespondingTransforms;

	Vector<mat4> modelMatrices;
	Vector<GLuint64> modelMainTextures;
	Vector<GLuint64> modelHeightTextures;
	
	Shader* shader;
	Mesh* mesh;

	bool notifyLayoutChange;

};

