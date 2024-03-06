#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>

void winSignal(bool *);

void winMain(GLFWwindow *win)
{
	ImGui::Begin("Последовательный обнаружитель сигнала");

	static bool winSignalOpen = false;
	if (ImGui::Button("Сигнал")) winSignalOpen = true;
	if (winSignalOpen) winSignal(&winSignalOpen);

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
