#include <random>
#include <vector>
#include <Signal.hpp>
#include <imgui.h>
#include <implot.h>

class Noise : public Signal {
public:
	Noise(int length = 1000) :
			Signal(length)
	{}

	void Setup() override
	{
		ImGui::Begin("Фоновый шум", &setupOpen);

		const char *noiseTypes[] = { "Гауссовский" };
		static int noiseType = 0;

		static bool changed = true;
		static float mean = 0;
		static float stddev = 0;

		// TODO change to setting Detector values
		static std::vector<float> x(length), y(length);
		if (changed) {
			// TODO Check for valid input
			// Setup distribution generator
#ifndef DEBUG
			static std::random_device rd{};
			static std::mt19937 gen{rd()};
#else
			static std::mt19937 gen{};
#endif
			std::normal_distribution<float> randn{mean, stddev};
			for (int i = 0; i < length; i++) {
				x[i] = i;
				y[i] = randn(gen);
			}
			changed = false;
		}

		if (ImPlot::BeginPlot("Фоновый шум")) {
			ImPlot::SetupAxes("Отсчёт k", "Уровень n(k)");
			ImPlot::PlotLine("Новый", x.data(), y.data(), length);
			ImPlot::PlotLine("Старый", this->x.data(), this->y.data(), length);
			ImPlot::EndPlot();
		}


		ImGui::Combo("Вид шума", &noiseType, noiseTypes, IM_ARRAYSIZE(noiseTypes));
		changed |= ImGui::SliderFloat("Среднее", &mean, -10, 10);
		changed |= ImGui::SliderFloat("СКО", &stddev, 0, 10);

		if (ImGui::Button("Применить")) {
			this->y = y;
		}

		ImGui::End();
	}
};
