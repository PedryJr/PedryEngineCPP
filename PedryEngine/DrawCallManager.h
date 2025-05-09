#pragma once
class DrawCallManager
{

public:
	static void ExecuteDrawCalls();
	static void AddDrawCall(Mesh* mesh, Shader* shader, Transform* transform);

	static Vector<DrawCallBatch> drawCalls;

};