#pragma once

#include <cmath>
#include <imgui.hpp>
#include <WindowControlled.hpp>
#include <InfoSignal.hpp>
#include <NoiseSignal.hpp>

class DetectorSequential : public WindowControlled {
private:
	// Detection parameters
	float qvals[3];
	float q0;
	float q;
	float α;
	float β;
	int nmax;
	int expCount;
	const float time_step;

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
		float A = logf((1 - β) / α) * σ / q0;
		float B = logf(β / (1 - α)) * σ / q0;
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

	void checkConstraints()
	{
		float qmin = 0.001;
		float pmin = 0.001;
		if (qvals[0] < qmin) qvals[0] = qmin;
		if (qvals[1] < qmin) qvals[1] = qmin;
		if (qvals[2] < qmin) qvals[2] = qmin;
		if (q0 < qmin) q0 = qmin;
		if (q < qmin) q = qmin;
		if (α < pmin) α = pmin;
		if (α > 1) α = 1;
		if (β < pmin) β = pmin;
		if (β > 1) β = 1;
		if (nmax < 1) nmax = 1;
		if (expCount < 1) expCount = 1;
	}

	void Setup() override
	{
		int length = qs.size();

		ImGui::SeparatorText("Сигнал");
		s.Show("Сигнал", true);
		ImGui::SeparatorText("Шум");
		n.Show("Шум", true);
		q = sqrt(s().GetEnergy() / n().GetEnergy());
		ImGui::SeparatorText("Обнаружитель");
		ChangeSnrRange();
		ImGui::SliderFloat("Расчётное ОСШ", &q0, 0.01, 10);
		ChangeSnrReal();
		ChangeBorders();
		ImGui::SliderInt("Максимальная выборка", &nmax, 1, 100);
		ImGui::SliderInt("Число опытов", &expCount, 100, 10000);
		checkConstraints();
	}

public:
	DetectorSequential() : WindowControlled("Последовательный обнаружитель"),
			time_step(0.001),
			s(time_step),
			n(time_step),
			meanCount(0),
			meanDetects(0),
			as(0),
			bs(0),
			zs(0),
			q0(1),
			q(q0),
			α(0.01),
			β(0.01),
			nmax(50),
			expCount(5000),
			qs(0),
			ns(0),
			ps(0),
			qvals{0.1, 0.5, 3.0}
	{}

	int GetNMax() const { return nmax; }

	std::vector<float> GetNoise(int count)
	{
		return n().Generate(count, time_step);
	}

	std::vector<float> GetSignal(int count)
	{
		return s().Generate(count, time_step);
	}


	void MakeStats() { genBorders(); detect<true>(); }
	const std::vector<float> & GetStats() { return zs; }
	std::vector<float> & GetBorderA()
	{
		if (as.empty() || bs.empty()) genBorders();
		return as;
	}
	std::vector<float> & GetBorderB()
	{
		if (as.empty() || bs.empty()) genBorders();
		return bs;
	}

	void MakeCharacteristics() { makePlots(qvals); }
	const std::vector<float> & GetQs() { return qs; }
	const std::vector<float> & GetNs() { return ns; }
	const std::vector<float> & GetPs() { return ps; }

	void SetSnr(float q) { makeE(q); }
	float GetSnr() { return q0; }

	void ChangeSnrReal()
	{
		if (ImGui::SliderFloat("Реальное ОСШ q", &q, 0.001f, 2*q0, "%.3f")) {
			SetSnr(q);
		}
	}

	void ChangeBorders()
	{
		bool bordersChanged = ImGui::SliderFloat("Вероятность ЛТ", &α, 0.001f,
				1, "%.3f", ImGuiSliderFlags_Logarithmic);
		bordersChanged |= ImGui::SliderFloat("Вероятность ПС", &β, 0.001f,
				1, "%.3f", ImGuiSliderFlags_Logarithmic);
		if (bordersChanged) genBorders();
	}

	void ChangeSnrRange()
	{
		ImGui::SliderFloat3("Значения ОСШ", qvals, 0.01, 5, "%.2f", ImGuiSliderFlags_Logarithmic);
		//ImGui::SameLine();
		//ImGui::HelpMarker("Значения, для которых будут строится зависимости: начальное, шаг, конечное.\nРаботает аналогично Matlab-синтаксису `q = start:step:end`");
	}
};

