#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>

#include <DetectorSequential.hpp>
#include <DetectorSequentialHistograms.hpp>

void winMain(GLFWwindow *win)
{
	ImGui::Begin("Последовательный обнаружитель сигнала");

	static DetectorSequential detector;
	static DetectorSequentialHistograms hists(detector);

	detector.Show("Настройки");
	hists.Show("Гистограммы");

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
