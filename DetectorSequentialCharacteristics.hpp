#pragma once

#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>
#include <imgui.h>
#include <implot.h>

class DetectorSequentialCharacteristics : public WindowControlled {
private:
	DetectorSequential &detector;

	virtual void Setup() override
	{
		static bool showQ0 = false;
		static int toBuild = 2; // Three-state flag: Wait->Warn->Process
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
			ImPlot::PlotLine("Вероятность обнаружения", q.data(), p.data(), q.size());
			if (showQ0) ImPlot::TagX(q0, ImVec4(1,1,0,1), "q0: %0.1f", q0);
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Объём выборки", ImVec2(-1,0), flags)) {
			ImPlot::SetupAxes("ОСШ q", "Средняя выборка n");
			ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, INFINITY);
			ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, INFINITY);
			ImPlot::PlotLine("Средний объём", q.data(), n.data(), q.size());
			if (showQ0) ImPlot::TagX(q0, ImVec4(1,1,0,1), "q0: %0.1f", q0);
			ImPlot::EndPlot();
		}
		if (toBuild == 2) {
			if (ImGui::Button("Построить")) {
				// Change button name first, and only then go to processing
				toBuild = 1;
			}
		} else if (toBuild == 1) {
			ImGui::BeginDisabled();
			ImGui::Button("Расчёт...");
			ImGui::EndDisabled();
			toBuild = 0;
		} else if (toBuild == 0) {
			detector.MakeCharacteristics();
			toBuild = 2;
		}
		ImGui::SameLine();
		ImGui::Checkbox("Показать расчётное ОСШ q0", &showQ0);
		detector.ChangeSnrRange();
		detector.ChangeBorders();
	}

public:
	DetectorSequentialCharacteristics(DetectorSequential &d) :
			WindowControlled("Характеристика обнаружителя"),
			detector{d}
	{}
};
