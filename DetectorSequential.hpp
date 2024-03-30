#pragma once

#include <cmath>
#include <imgui.h>
#include <WindowControlled.hpp>
#include <InfoSignal.hpp>
#include <NoiseSignal.hpp>

class DetectorSequential : public WindowControlled {
private:
	InfoSignal s;
	NoiseSignal n;

	float qvals[3];
	float q0;
	float α;
	float β;
	int nmax;
	int expCount;
	const float time_step;

	// Modelling results
	float meanCount;
	float meanDetects;
	std::vector<float> as;
	std::vector<float> bs;
	std::vector<float> zs;
	std::vector<float> qs;
	std::vector<float> ns;
	std::vector<float> ps;

	template<bool rec = false>
	int detect()
	{
		float t = 0, Z = 0;
		if (rec) {
			zs.clear();
		}
		for (int i = 0; i < nmax; i++) {
			float sp = s().Generate(t);
			float np = n().Generate(t);
			float y = sp + np;
			// TODO Calculate in case of other noises
			float z = y;
			t += time_step;
			Z += z;
			if (rec) {
				zs.push_back(Z);
			}
			if (Z > as[i]) return  (i+1);
			if (Z < bs[i]) return -(i+1);
		}
		const float finalBorder = nmax * q0 / 2;
		if (Z > finalBorder) return  nmax;
		else                 return -nmax;
	}

	void measure()
	{
		int totalCount = 0;
		int totalDetects = 0;
		for (int k = 0; k < expCount; k++) {
			int d = detect();
			totalCount += abs(d);
			if (d > 0) totalDetects++;
		}
		meanCount = float(totalCount) / expCount;
		meanDetects = float(totalDetects) / expCount;
	}

	void genBorders()
	{
		as.clear();
		bs.clear();
		float σ = sqrt(n().GetEnergy());
		//float E = s().GetEnergy();
		float A = log10f((1 - β) / α) * σ / q0;
		float B = log10f(β / (1 - α)) * σ / q0;
		float borderStep = q0 / (2 * σ);
		for (int i = 0; i < nmax; i++) {
			A += borderStep;
			B += borderStep;
			as.push_back(A);
			bs.push_back(B);
		}
	}

	void genQList(float q_start, float q_step, float q_stop)
	{
		qs.clear();
		float q = q_start;
		do {
			qs.push_back(q);
			q += q_step;
		} while (q <= q_stop);
	}

	void makeE(float q)
	{
		// q = s / σ  =>  q² = E / σ²  =>  E = q² * σ²
		float σ2 = sqrt(n().GetEnergy());
		float E = q * q * σ2;
		s().SetEnergy(E);
	}

	void makePlots(float qvals[3])
	{
		genQList(qvals[0], qvals[1], qvals[2]);
		genBorders();
		ns.clear();
		ps.clear();
		for (auto q : qs) {
			makeE(q);
			measure();
			ns.push_back(meanCount);
			ps.push_back(meanDetects);
		}
		makeE(q0);
	}

	void Setup() override
	{
		int length = qs.size();

		// TODO Show q barriers?
		static bool plots = false;
		ImGui::Checkbox("Графики характеристик", &plots);
		if (plots) {
		if (ImPlot::BeginPlot("Характеристика обнаружения")) {
			ImPlot::SetupAxes("ОСШ q", "Вероятность ПО p");
			ImPlot::PlotLine("p(q)", qs.data(), ps.data(), length);
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Характеристик выборки")) {
			ImPlot::SetupAxes("ОСШ q", "Средняя выборка n");
			ImPlot::PlotLine("n(q)", qs.data(), ns.data(), length);
			ImPlot::EndPlot();
		}
		}

		ImGui::SeparatorText("Сигнал");
		s.Show("Сигнал", true);
		ImGui::SeparatorText("Шум");
		n.Show("Шум", true);
		ImGui::SeparatorText("Обнаружитель");
		ImGui::SliderFloat3("Значения ОСШ", qvals, 0.1, 10, "%.2f", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderFloat("Расчётное ОСШ", &q0, 0, 10)
		ImGui::SliderFloat("Вероятность ЛТ", &α, 0.001f, 1, "%.3f", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderFloat("Вероятность ПС", &β, 0.001f, 1, "%.3f", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderInt("Максимальный объём выборки", &nmax, 1, 100);
		ImGui::SliderInt("Число опытов", &expCount, 100, 10000);
	}

public:
	DetectorSequential() : WindowControlled("Последовательный обнаружитель"),
			time_step(0.001),
			s(time_step),
			n(time_step),
			q0(1),
			α(0.01),
			β(0.01),
			nmax(50),
			expCount(5000),
			qs(0),
			ns(0),
			ps(0),
			qvals{0.1, 0.5, 3.0}
	{}

	std::vector<float> GetNoise(int count)
	{
		return n().Generate(count, time_step);
	}

	std::vector<float> GetSignal(int count)
	{
		return s().Generate(count, time_step);
	}

	float GetBorderA() { return log10f((1 - β) / α) * sqrt(n().GetEnergy()) / q0 + (q0 / 2); }
	float GetBorderB() { return log10f(β / (1 - α)) * sqrt(n().GetEnergy()) / q0 + (q0 / 2); }

	void MakeStats() { genBorders(); detect<true>(); }
	const std::vector<float> & GetStats() { return zs; }
	const std::vector<float> & GetBorderAv() { return as; }
	const std::vector<float> & GetBorderBv() { return bs; }
};

