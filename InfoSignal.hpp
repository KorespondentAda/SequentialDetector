#pragma once

#include <cmath>
#include <imgui.h>
#include <implot.h>
#include <WindowControlled.hpp>
#include <SignalGenerator.hpp>

class InfoSignal : public WindowControlled {
private:
	SignalGenerator gen;
	const float time_step;

	virtual void Setup() override
	{
		ImGui::TextWrapped("Установка вида обнаруживаемого сигнала и его параметров");
		static bool plot = false;
		ImGui::Checkbox("График сигнала", &plot);

		// TODO Delete or move under debug option
		static bool changed = true;
		if (plot) {
			const int length = 100;
			static float x[length], y[length];
			auto &g = gen();
			if (changed) {
				float t = 0;
				for (int i = 0; i < length; i++) {
					x[i] = t;
					t += time_step;
					y[i] = g.Generate(x[i]);
				}
				changed = false;
			}

			if (ImPlot::BeginPlot("Искомый сигнал")) {
				ImPlot::SetupAxes("Время t", "Уровень s(t)");
				ImPlot::PlotLine("Сигнал", x, y, length);
				ImPlot::EndPlot();
			}
		}

		changed |= gen.Selector("Вид сигнала");
		changed |= gen.Configure();
	}

public:
	InfoSignal(float tstep = 0.001) :
			WindowControlled("Обнаруживаемый сигнал"),
			gen(1),
			time_step(tstep)
	{}

	Generator & operator()()
	{
		return gen();
	}
};

