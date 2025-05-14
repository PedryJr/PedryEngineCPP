#pragma once
#include "PedryEngine.h"
#include "Engine.h"

int main()
{

	Engine engine = Engine();
	engine.Run();

	//std::thread(&Engine::Run, Engine()).join();

}