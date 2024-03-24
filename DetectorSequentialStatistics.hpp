#pragma once

#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>
#include <implot.h>

class DetectorSequentialStatistics : public WindowControlled {
private:
	DetectorSequential &detector;

	virtual void Setup() override
	{
		if (ImGui::Button("Обнаружить")) detector.MakeStats();
		if (ImPlot::BeginPlot("Статистика")) {
			auto s = detector.GetStats();
			auto a = detector.GetBorderAv();
			auto b = detector.GetBorderBv();
			ImPlot::PlotLine("Статистика", s.data(), s.size());
			ImPlot::PlotLine("Верхний порог", a.data(), a.size());
			ImPlot::PlotLine("Нижний порог", b.data(), b.size());
			ImPlot::EndPlot();
		}
	}

public:
	DetectorSequentialStatistics(DetectorSequential &d) :
			WindowControlled("Решающая статистика"),
			detector{d}
	{}
};
