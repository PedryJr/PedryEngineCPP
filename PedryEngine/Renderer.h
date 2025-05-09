#pragma once
class Renderer
{

public:
	Renderer();
	~Renderer();

	static void Initialize();

	static void DrawShape(VisualShape& visualShape, Vector<vec4>& positions);
	static void DrawBatch(DrawCallBatch* batch);
	static void SwapBuffer();
	static bool ShouldTerminate();
	static void SetupWindowHints();
	static void OpenGLEnableCaps();
	static void DisplaySettings();
	static void Finalize();
	static void InitializeMonitor();
	static void InitializeWindow();

	static GLFWwindow* GetWindow();

private:

	static GLFWwindow* window;
	static GLFWmonitor* monitor;
	static int width, height;

};