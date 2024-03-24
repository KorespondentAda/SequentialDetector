#pragma once

#include <vector>
#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>

class DetectorSequentialHistograms : public WindowControlled {
private:
	DetectorSequential &detector;
	std::vector<float> s, n, y;
	const int count;

	virtual void Setup() override
	{
		static int bins = 50;
		static bool online = false;
		if (ImPlot::BeginPlot("Гистограммы")) {
			float borders[] = {detector.GetBorderA(), detector.GetBorderB()};
			ImPlot::PlotHistogram("Сигнал есть", y.data(), y.size(), bins, 1, ImPlotRange(), ImPlotHistogramFlags_Density);
			ImPlot::PlotHistogram("Сигнала нет", n.data(), n.size(), bins, 1, ImPlotRange(), ImPlotHistogramFlags_Density);
			ImPlot::PlotInfLines("Пороги", borders, 2);
			ImPlot::EndPlot();
		}
		if (ImGui::Button("Построить") || online) {
			s = detector.GetSignal(count);
			n = detector.GetNoise(count);
			for (int i = 0; i < count; i++) {
				y[i] = s[i] + n[i];
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("Перестраивать автоматически", &online);
		ImGui::SliderInt("Число карманов", &bins, 10, 100);
	}

public:
	DetectorSequentialHistograms(DetectorSequential &d, int count = 10000) :
			WindowControlled("Гистограммы колебаний"),
			detector{d}, count(count), s(count), n(count), y(count)
	{}
};
