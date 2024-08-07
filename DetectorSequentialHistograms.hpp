#pragma once

#include <vector>
#include <fstream>
#include <WindowControlled.hpp>
#include <DetectorSequential.hpp>
#include <ImGuiFileDialog.h>

class DetectorSequentialHistograms final : public WindowControlled {
private:
	DetectorSequential &detector;
	const int count;
	std::vector<float> s, n, y;
	IGFD::FileDialog saveDialog;

	virtual void Setup() override
	{
		static int bins = 50;
		static int watchLen = 1;
		static bool online = false;
		int watchLenMax = detector.GetNMax();
		if (watchLen > watchLenMax) watchLen = watchLenMax;
		if (ImPlot::BeginPlot("Гистограммы")) {
			float borders[] = {detector.GetBorderA()[watchLen-1], detector.GetBorderB()[watchLen-1]};
			ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, INFINITY);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 0.5f);
			ImPlot::PlotHistogram("Сигнал есть", y.data(), y.size(), bins, 1, ImPlotRange(), ImPlotHistogramFlags_Density);
			ImPlot::PlotHistogram("Сигнала нет", n.data(), n.size(), bins, 1, ImPlotRange(), ImPlotHistogramFlags_Density);
			ImPlot::PopStyleVar();
			ImPlot::PopStyleVar();
			ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 3.0f);
			ImPlot::PlotInfLines("Пороги", borders, 2);
			ImPlot::EndPlot();
		}
		if (ImGui::Button("Обновить гистограммы") || online) {
			s = detector.GetSignal(count);
			n = detector.GetNoise(count);
			for (int i = 0; i < count; i++) {
				n[i] *= std::sqrt(watchLen);
				y[i] = watchLen * s[i] + n[i];
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Сохранить...")) {
			saveDialog.OpenDialog(
					"SaveHistograms",
					"Сохранить точки",
					".csv",
					{
						.path = ".",
						.fileName = "samples.csv",
						.filePathName = {},
						.sidePane = {},
						.userFileAttributes = {}
					}
					);
		}
		ImGui::Checkbox("Перестраивать автоматически", &online);
		ImGui::SliderInt("Число карманов", &bins, 10, 100);
		ImGui::SliderInt("Шаг обнаружения", &watchLen, 1, watchLenMax);
		detector.ChangeSnrReal();
		if (saveDialog.Display("SaveHistograms")) {
			if (saveDialog.IsOk()) {
				auto selectedFilePathName = saveDialog.GetFilePathName();
				std::ofstream of(selectedFilePathName);
				of << "noise,signal" << std::endl;
				for (size_t i = 0; i < n.size(); i++) {
					of << n[i] << ',' << s[i] << std::endl;
				}
			}
			saveDialog.Close();
		}
	}

public:
	DetectorSequentialHistograms(DetectorSequential &d, int count = 10000) :
			WindowControlled("Гистограммы колебаний"),
			detector{d}, count(count), s(count), n(count), y(count)
	{}
};
