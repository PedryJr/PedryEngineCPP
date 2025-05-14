#pragma once
class Engine
{

public:

	void Run();
	bool shouldTerminate;
	static ParallelIterator<vec4> vecIterator;

	static FORCE_INLINE GLdouble InnerDeltaTime() { return deltaTime; }

private:
	Clock timer;

	static GLdouble deltaTime;
	Game* game = nullptr;

	void InitializeEngine();
	void UpdateLoop();
	void InputUpdate();
	void TimelineUpdate();
	void GraphicsUpdate();
	void GameUpdate();


};