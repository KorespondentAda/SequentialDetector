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
		if (ImGui::Button("Построить")) detector.MakeCharacteristics();
		if (ImPlot::BeginPlot("Характеристика обнаружения")) {
			auto q = detector.GetQs();
			auto p = detector.GetPs();
			// TODO Убрать легенды
			ImPlot::PlotLine("Вероятность обнаружения", q.data(), p.data(), q.size());
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Объём выборки")) {
			auto q = detector.GetQs();
			auto n = detector.GetNs();
			ImPlot::PlotLine("Средний объём", q.data(), n.data(), q.size());
			ImPlot::EndPlot();
		}
	}

public:
	DetectorSequentialCharacteristics(DetectorSequential &d) :
			WindowControlled("Характеристика обнаружителя"),
			detector{d}
	{}
};
