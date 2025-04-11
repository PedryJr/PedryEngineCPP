#pragma once
#ifndef Engine_H
#define Engine_H
class Engine
{

public:

	void Run();
	bool shouldTerminate;
	static ParallelIterator<vec4> vecIterator;
	static GLfloat InnerDeltaTime();

private:
	static GLfloat deltaTime;


};
#endif // PARALLEL_ITERATOR_H