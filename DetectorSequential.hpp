#pragma once

#include <cmath>
#include <imgui.h>
#include <Signal.hpp>
#include <Noise.hpp>

#define BTN_SETUP(title, sig) \
	if (ImGui::Button(title)) sig.OpenSetup(); \
	if (sig.isSetupOpen()) sig.Setup();

class DetectorSequential {
private:
	Signal s;
	Noise n;
	float q0;
	float α;
	float β;
	int nmax;
	int expCount;

	bool opened;

public:
	DetectorSequential() :
			s(1000),
			n(1000),
			q0(1),
			α(0.01),
			β(0.01),
			nmax(50),
			expCount(5000),
			opened(false)
	{}

	void Open() { opened = true; }
	bool isOpen() { return opened; }
	void Show()
	{
		ImGui::Begin("Последовательный обнаружитель", &opened);
		BTN_SETUP("Сигнал", s) ImGui::SameLine();
		BTN_SETUP("Шум", n)

		static float meanCount;
		static float meanDetects;

		if (ImGui::Button("Построить характеристики")) {
			float σ = n.getStdDev();
			float A0 = log10f((1 - β) / α) * σ / q0;
			float B0 = log10f(β / (1 - α)) * σ / q0;
			float borderStep = s.getEnergy() / 2;
			int totalCount = 0;
			int totalDetects = 0;
			for (int k = 0; k < expCount; k++) {
				float Z = 0;
				float A = A0;
				float B = B0;
				n.Regen();
				for (int i = 0; i < nmax; i++) {
					float y = s[i] + n[i];
					float z = y * s[i];

					Z += z;
					A += borderStep;
					B += borderStep;
					if (Z > A || Z < B) {
						totalCount += (i+1);
						totalDetects += (Z > A ? 1 : 0);
						break;
					}
				}
			}
			meanCount = float(totalCount) / expCount;
			meanDetects = float(totalDetects) / expCount;
		}

		ImGui::Text("Средний объём выборки: %.1f; Вероятность обнаружения: %.2f",
				meanCount, meanDetects);

		ImGui::SliderFloat("Расчётное ОСШ", &q0, 0, 5);
		ImGui::SliderFloat("Вероятность ЛТ", &α, 0, 1);
		ImGui::SliderFloat("Вероятность ПС", &β, 0, 1);
		ImGui::SliderInt("Максимальный объём выборки", &nmax, 1, 100);
		ImGui::SliderInt("Число опытов", &expCount, 100, 10000);

		ImGui::End();
	}
};

