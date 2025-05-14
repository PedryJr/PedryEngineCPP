#pragma once
class RenderSystem
{

public:
	RenderSystem();
	~RenderSystem();

	static void Initialize();

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
