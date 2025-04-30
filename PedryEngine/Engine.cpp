#pragma once
#include "PedryEngine.h"

void Engine::Run()
{

	glfwInit();

	Renderer renderer = Renderer();
	Controller controller = Controller();
	VisualShape shape = VisualShape();

	Vector<vec4> positions(INSTANCING_TEST);

	vec4 defaultValue = { -(GLfloat).0f, (GLfloat).0f, (GLfloat).0f, (GLfloat).0f };

	vecIterator.setFunction([defaultValue](vec4& value) {

		value = defaultValue;

	});

	vecIterator.process(&positions[0], INSTANCING_TEST);

	vecIterator.waitForCompletion();

	Clock timer = TimeNow;

	while (renderer.ShouldTerminate())
	{

		glClear(GL_COLOR_BUFFER_BIT);

		renderer.DrawShape(shape, positions);
		renderer.SwapBuffer();

		glfwPollEvents();

		Clock newTimer = TimeNow;
		deltaTime = Duration(timer, newTimer);
		timer = newTimer;

		controller.ScanInput(renderer.GetWindow(), positions);
	}
}

GLdouble Engine::InnerDeltaTime()
{
	return deltaTime;
}

GLdouble Engine::deltaTime = 0.0;

ParallelIterator<vec4> Engine::vecIterator;