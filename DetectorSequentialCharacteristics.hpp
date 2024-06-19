#pragma once

#include <future>
#include <chrono>
#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>
#include <imgui.h>
#include <implot.h>

class DetectorSequentialCharacteristics : public WindowControlled {
private:
	DetectorSequential &detector;
	std::future<void> calcFuture;

	virtual void Setup() override
	{
		using namespace std::chrono_literals;
		static bool showQ0 = false;
		static const auto flags = ImPlotFlags_NoLegend;
		auto q0 = detector.GetSnr();
		auto q = detector.GetQs();
		auto p = detector.GetPs();
		auto n = detector.GetNs();
		if (ImPlot::BeginPlot("Характеристика обнаружения", ImVec2(-1,0), flags)) {
			const ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit;
			ImPlot::SetupAxes("ОСШ q", "Вероятность ПО p");
			ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, INFINITY);
			ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 1);
			if (!calcFuture.valid())
				ImPlot::PlotLine("Вероятность обнаружения", q.data(), p.data(), q.size());
			if (showQ0) {
				ImPlot::TagX(q0, ImVec4(1,1,0,1), "q0: %0.1f", q0);
				ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 3.0f);
				ImPlot::PlotInfLines("q0", &q0, 1);
			}
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Объём выборки", ImVec2(-1,0), flags)) {
			ImPlot::SetupAxes("ОСШ q", "Средняя выборка n");
			ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, INFINITY);
			ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, INFINITY);
			if (!calcFuture.valid())
				ImPlot::PlotLine("Средний объём", q.data(), n.data(), q.size());
			if (showQ0) {
				ImPlot::TagX(q0, ImVec4(1,1,0,1), "q0: %0.1f", q0);
				ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 3.0f);
				ImPlot::PlotInfLines("q0", &q0, 1);
			}
			ImPlot::EndPlot();
		}
		if (calcFuture.valid()) {
			// TODO Press for stop calculation
			ImGui::BeginDisabled();
			ImGui::Button("Расчёт...");
			ImGui::EndDisabled();
			if (calcFuture.wait_for(0ms) == std::future_status::ready) {
				calcFuture.get();
			}
		} else {
			if (ImGui::Button("Построить")) {
				calcFuture = std::async(std::launch::async,
						[&](){detector.MakeCharacteristics();});
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("Показать расчётное ОСШ q0", &showQ0);
		detector.ChangeSnrRange();
		detector.ChangeExpCount();
		detector.ChangeSnrWait();
		detector.ChangeBorders();
	}

public:
	DetectorSequentialCharacteristics(DetectorSequential &d) :
			WindowControlled("Характеристика обнаружителя"),
			detector{d}
	{}
};
