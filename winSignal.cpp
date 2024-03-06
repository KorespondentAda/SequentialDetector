#include <cmath>
#include <imgui.h>
#include <implot.h>

#define LENGTH 1000
#define PI 3.14

void winSignal(bool *isOpen)
{
	ImGui::Begin("Обнаруживаемый сигнал", isOpen);
	ImGui::Text("Установка вида обнаруживаемого сигнала и его параметров");

	const char *signalTypes[] = { "Гармонический" };
	static int signalType = 0;
	ImGui::Combo("Вид сигнала", &signalType, signalTypes, IM_ARRAYSIZE(signalTypes));

	static bool changed = true;
	static float freq = 10;
	changed |= ImGui::InputFloat("Частота", &freq);
	static float amp = 1;
	changed |= ImGui::InputFloat("Амплитуда", &amp);
	static float phase = 0;
	changed |= ImGui::InputFloat("Фаза", &phase);

	// TODO change to setting Detector values
	static float x[LENGTH], y[LENGTH];
	if (changed) {
		for (int i = 0; i < LENGTH; i++) {
			x[i] = i;
			y[i] = amp * cosf(2 * PI * freq * i + phase);
		}
		changed = false;
	}

	if (ImPlot::BeginPlot("Искомый сигнал")) {
		ImPlot::SetupAxes("Отсчёт k", "Уровень s(k)");
		ImPlot::PlotLine("Сигнал", x, y, LENGTH);
		ImPlot::EndPlot();
	}

	ImGui::End();
}
