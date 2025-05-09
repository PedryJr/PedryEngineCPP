#pragma once
class Engine
{

public:

	void Run();
	bool shouldTerminate;
	static ParallelIterator<vec4> vecIterator;

	static FORCE_INLINE GLdouble InnerDeltaTime() { return deltaTime; }

private:
	static GLdouble deltaTime;


};