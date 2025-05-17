#pragma once
class RenderSystem
{

public:

	static void Initialize();

	static void DrawBatchShadow(DrawCallBatch* batch);
	static void DrawBatchNormal(DrawCallBatch* batch);
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
