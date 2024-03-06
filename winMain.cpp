#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>

#include <Signal.hpp>
#include <Noise.hpp>

#define BTN_SETUP(title, sig) \
	if (ImGui::Button(title)) sig.OpenSetup(); \
	if (sig.isSetupOpen()) sig.Setup();

void winMain(GLFWwindow *win)
{
	ImGui::Begin("Последовательный обнаружитель сигнала");
	static Signal signal;
	static Noise noise;

	BTN_SETUP("Сигнал", signal)
	BTN_SETUP("Шум", noise)

	if (ImGui::Button("Закрыть")) {
		glfwSetWindowShouldClose(win, true);
	}

	static bool demoImgui = false;
	if (ImGui::Button("Demo ImGui")) demoImgui = true;
	if (demoImgui) ImGui::ShowDemoWindow(&demoImgui);

	static bool demoImplot = false;
	if (ImGui::Button("Demo ImPlot")) demoImplot = true;
	if (demoImplot) ImPlot::ShowDemoWindow(&demoImplot);

	ImGui::End();
}
