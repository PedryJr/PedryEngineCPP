#pragma once
#ifndef Renderer_H
#define Renderer_H
class Renderer
{

public:
	Renderer();
	~Renderer();

	void DrawShape(VisualShape& visualShape, Vector<vec4>& positions);
	void SwapBuffer();
	bool ShouldTerminate();
	void SetupWindowHints();
	void OpenGLEnableCaps();
	void DisplaySettings();
	void Finalize();
	void InitializeMonitor();
	void InitializeWindow();

	GLFWwindow* GetWindow();

private:

	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;
	int width, height = 0;

};
#endif