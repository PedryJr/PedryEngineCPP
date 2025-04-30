#pragma once
#ifndef Engine_H
#define Engine_H
class Engine
{

public:

	void Run();
	bool shouldTerminate;
	static ParallelIterator<vec4> vecIterator;
	static GLdouble InnerDeltaTime();

private:
	static GLdouble deltaTime;


};
#endif // PARALLEL_ITERATOR_H