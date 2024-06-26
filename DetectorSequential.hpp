#pragma once

#include <cmath>
#include <imgui.hpp>
#include <WindowControlled.hpp>
#include <InfoSignal.hpp>
#include <NoiseSignal.hpp>

#ifdef MEASURE_TIME
#include <chrono>
#endif

class DetectorSequential : public WindowControlled {
private:
#ifdef MEASURE_TIME
	// Building time
	std::chrono::milliseconds exTime;
#endif // MEASURE_TIME
	// Detection parameters
	float qvals[3];
	float q0;
	float q;
	float α;
	float β;
	int nmax;
	int expCount;

	// Signal sources
	InfoSignal s;
	NoiseSignal n;

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
		float Z = 0;
		if (rec) {
			zs.clear();
		}
		auto sp = s().Generate(nmax);
		auto np = n().Generate(nmax);
		for (int i = 0; i < nmax; i++) {
			float y = sp[i] + np[i];
			// TODO Calculate in case of other noises
			Z += calcStat(y);
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
		float A = logf((1 - β) / α) * σ / q0;
		float B = logf(β / (1 - α)) * σ / q0;
		// q0 * σ / 2
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
		float σ2 = n().GetEnergy();
		float E = q * q * σ2;
		s().SetEnergy(E);
	}

	float calcStat(float y) const { return y; }

	void makePlots()
	{
		genQList(qvals[0], qvals[1], qvals[2]);
		genBorders();
		ns.clear();
		ps.clear();
		auto qOld = q;
		for (auto q : qs) {
			makeE(q);
			measure();
			ns.push_back(meanCount);
			ps.push_back(meanDetects);
		}
		makeE(q = qOld);
	}

	void checkConstraints()
	{
		float qmin = 0.001;
		float pmin = 0.001;
		if (qvals[0] < 0) qvals[0] = 0;
		if (qvals[1] < qmin) qvals[1] = qmin;
		if (qvals[2] < qmin) qvals[2] = qmin;
		if (q0 < qmin) q0 = qmin;
		if (q < 0) q = 0;
		if (α < pmin) α = pmin;
		if (α > 1) α = 1;
		if (β < pmin) β = pmin;
		if (β > 1) β = 1;
		if (nmax < 1) nmax = 1;
		if (expCount < 1) expCount = 1;
	}

	void Setup() override
	{
		ImGui::SeparatorText("Сигнал");
		s.Show("Сигнал", true);
		ImGui::SeparatorText("Шум");
		n.Show("Шум", true);
		q = sqrt(s().GetEnergy() / n().GetEnergy());
		ImGui::SeparatorText("Обнаружитель");
		ChangeSnrWait();
		ChangeSnrReal();
		ChangeBorders();
		ChangeNMax();
		checkConstraints();
#ifdef MEASURE_TIME
		ImGui::Text("Длительность построения: %.3f с", exTime.count() / 1000.0, 0, 100);
#endif
	}

public:
	using Samples = std::vector<float>;

	DetectorSequential() : WindowControlled("Последовательный обнаружитель"),
			qvals{0.0, 0.05, 3.0},
			q0(1),
			q(q0),
			α(0.01),
			β(0.01),
			nmax(50),
			expCount(5000)
	{}

	int GetNMax() const { return nmax; }

	Samples GetNoise(int count)
	{
		return n().Generate(count);
	}
	Samples GetSignal(int count)
	{
		return s().Generate(count);
	}

	void MakeStats() { genBorders(); detect<true>(); }
	const Samples & GetStats() const { return zs; }
	const Samples & GetBorderA()
	{
		if (as.empty() || bs.empty()) genBorders();
		return as;
	}
	const Samples & GetBorderB()
	{
		if (as.empty() || bs.empty()) genBorders();
		return bs;
	}
	const Samples & GetBorderA() const { return as; }
	const Samples & GetBorderB() const { return bs; }

	void MakeCharacteristics() {
#ifdef MEASURE_TIME
		auto start{std::chrono::steady_clock::now()};
#endif // MEASURE_TIME
		makePlots();
#ifdef MEASURE_TIME
		auto end{std::chrono::steady_clock::now()};
		exTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
#endif // MEASURE_TIME
	}
	const Samples & GetQs() const { return qs; }
	const Samples & GetNs() const { return ns; }
	const Samples & GetPs() const { return ps; }

	void SetSnr(float q) { makeE(q); }
	float GetSnr() const { return q0; }

	void ChangeSnrRange()
	{
		ImGui::SliderFloat3("Значения ОСШ", qvals, 0.00, 5, "%.3f", ImGuiSliderFlags_Logarithmic);
		if (qvals[1] < 0.001) qvals[1] = 0.001;
		//ImGui::SameLine();
		//ImGui::HelpMarker("Значения, для которых будут строится зависимости: начальное, шаг, конечное.\nРаботает аналогично Matlab-синтаксису `q = start:step:end`");
	}
	void ChangeSnrWait()
	{
		ImGui::SliderFloat("Расчётное ОСШ", &q0, 0.01, 10);
	}
	void ChangeSnrReal()
	{
		if (ImGui::SliderFloat("Реальное ОСШ q", &q, 0.0f, 2*q0, "%.3f")) {
			SetSnr(q);
		}
	}
	void ChangeBorders()
	{
		bool bordersChanged = ImGui::SliderFloat("Вероятность ЛТ", &α,
				0.01f, 1, "%.2f", ImGuiSliderFlags_Logarithmic);
		bordersChanged |= ImGui::SliderFloat("Вероятность ПС", &β,
				0.01f, 1, "%.2f", ImGuiSliderFlags_Logarithmic);
		if (bordersChanged) genBorders();
	}
	void ChangeNMax()
	{
		ImGui::SliderInt("Максимальная выборка", &nmax,
				1, 10000, "%d", ImGuiSliderFlags_Logarithmic);
	}
	void ChangeExpCount()
	{
		ImGui::SliderInt("Число опытов", &expCount, 100, 10000);
	}
};

