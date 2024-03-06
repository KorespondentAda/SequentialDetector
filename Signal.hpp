#pragma once

#include <cmath>
#include <vector>
#include <imgui.h>
#include <implot.h>

#define PI 3.14

class Signal {
using arr = std::vector<float>;
protected:
	int length;
	arr x;
	arr y;

	bool setupOpen;

public:
	Signal(int length = 1000) :
			length(length),
			x(length),
			y(length),
			setupOpen(false)
	{
		for (int i = 0; i < length; i++) {
			x[i] = i;
		}
	}

	Signal(const Signal &s) = default;


	float getEnergy() const
	{
		float e = 0;
		for (auto el : y) {
			e += el * el;
		}
		return e / length;
	}

	bool isSetupOpen() { return setupOpen; }
	void OpenSetup() { setupOpen = true; }
	void CloseSetup() { setupOpen = false; }
	virtual void Setup()
	{
		ImGui::Begin("Обнаруживаемый сигнал", &setupOpen);
		ImGui::Text("Установка вида обнаруживаемого сигнала и его параметров");

		const char *signalTypes[] = { "Гармонический" };
		static int signalType = 0;

		static bool changed = true;
		static float freq = 0.02;
		static float amp = 3;
		static float phase = 0;

		// TODO change to setting Detector values
		static std::vector<float> x(length), y(length);
		if (changed) {
			// TODO Check for valid input
			for (int i = 0; i < length; i++) {
				x[i] = i;
				y[i] = amp * cosf(2 * PI * (1 + freq) * i + (PI * phase / 180));
			}
			changed = false;
		}

		if (ImPlot::BeginPlot("Искомый сигнал")) {
			ImPlot::SetupAxes("Отсчёт k", "Уровень s(k)");
			ImPlot::PlotLine("Новый", x.data(), y.data(), length);
			ImPlot::PlotLine("Старый", this->x.data(), this->y.data(), length);
			ImPlot::EndPlot();
		}


		ImGui::Combo("Вид сигнала", &signalType, signalTypes, IM_ARRAYSIZE(signalTypes));
		changed |= ImGui::SliderFloat("Частота", &freq, -0.1, 0.1);
		changed |= ImGui::SliderFloat("Амплитуда", &amp, 0, 10);
		changed |= ImGui::SliderFloat("Фаза", &phase, 0, 360);

		if (ImGui::Button("Применить")) {
			this->y = y;
		}

		ImGui::End();
	}

	float operator[](int i) { return y[i]; }
};

