#pragma once

struct Swizzle
{

	Vector<Transform*> corespondingTransforms;
	Vector<mat4> modelMatrices;
	Vector<GLuint64> modelMainTextures;
	Vector<GLuint64> modelHeightTextures;

};

class DrawCallBatch
{

public:
	DrawCallBatch(Shader* shader, Mesh* mesh);
	~DrawCallBatch();
	void AddBatchInstance(Transform& instanceSource, GLint batchIndex);
	
	Swizzle* renderSwizzle;
	Swizzle* gameSwizzle;

	Shader* shader;
	Mesh* mesh;

	bool notifyLayoutChange;

};

