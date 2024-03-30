#pragma once

#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>
#include <implot.h>

class DetectorSequentialStatistics : public WindowControlled {
private:
	DetectorSequential &detector;

	virtual void Setup() override
	{
		static std::vector<std::vector<float>> s;
		static std::vector<std::string> names;
		if (ImGui::Button("Построить")) {
			detector.MakeStats();
			s.push_back(detector.GetStats());
			names.push_back("Стат." + std::to_string(s.size()));
		}
		ImGui::SameLine();
		if (ImGui::Button("Очистить")) s.clear();
		if (ImPlot::BeginPlot("Статистика")) {
			auto a = detector.GetBorderAv();
			auto b = detector.GetBorderBv();
			for (int i = 0; i < s.size(); i++) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
				ImPlot::PlotLine(names[i].c_str(), s[i].data(), s[i].size());
			}
			// TODO Сделать жирнее
			//void SetNextLineStyle(const ImVec4& col, float weight) {
			// TODO Исправить ось - начать с 1, убрать дробные шаги, подпись,
			// без отрицательных
			ImPlot::PlotLine("##Верхний порог", a.data(), a.size());
			ImPlot::PlotLine("##Нижний порог", b.data(), b.size());
			ImPlot::EndPlot();
		}
		// TODO Выбор реального ОСШ
		// ImGui::SliderFloat();
	}

public:
	DetectorSequentialStatistics(DetectorSequential &d) :
			WindowControlled("Решающая статистика"),
			detector{d}
	{}
};
