#pragma once

#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>
#include <implot.h>

class DetectorSequentialStatistics : public WindowControlled {
private:
	DetectorSequential &detector;

	virtual void Setup() override
	{
		static std::vector<std::vector<float>> s(0);
		static std::vector<float> x(0);
		static std::vector<std::string> names(0);
		static int statCount = 0;
		if (ImPlot::BeginPlot("##Статистика")) {
			auto a = detector.GetBorderA();
			auto b = detector.GetBorderB();
			assert(a.size() == b.size());
			for (int i = x.size(); i < a.size(); i++) {
				x.push_back(i+1);
			}
			ImPlot::SetupAxes("Шаг обнаружения k", "Значение статистики z");
			ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 1, a.size());
			ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 10, INFINITY);
			ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
			for (int i = 0; i < s.size(); i++) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
				ImPlot::PlotLine(names[i].c_str(), x.data(), s[i].data(), s[i].size());
			}
			ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 3.0f);
			ImPlot::PlotLine("##Верхний порог", x.data(), a.data(), a.size());
			ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 3.0f);
			ImPlot::PlotLine("##Нижний порог", x.data(), b.data(), b.size());
			ImPlot::EndPlot();
		}
		if (ImGui::Button("Построить")) {
			detector.MakeStats();
			auto stat = detector.GetStats();
			s.push_back(stat);
			names.push_back(std::to_string(++statCount));
		}
		ImGui::SameLine();
		if (ImGui::Button("Очистить")) {
			s.clear();
			names.clear();
			statCount = 0;
			// TODO Clear line disabling
		}
		detector.ChangeSnrReal();
		detector.ChangeBorders();
	}

public:
	DetectorSequentialStatistics(DetectorSequential &d) :
			WindowControlled("Решающая статистика"),
			detector{d}
	{}
};
