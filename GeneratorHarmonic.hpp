#pragma once

#include <algorithm>
#include <cmath>
#include <Generator.hpp>

class GeneratorHarmonic : public Generator {
private:
	float amplitude;
	float frequency;
	float phase;

public:
	GeneratorHarmonic(float amplitude, float frequency, float phase) :
			Generator("Гармонический"),
			amplitude(amplitude),
			frequency(frequency),
			phase(phase)
	{}

	std::vector<float> Generate(std::vector<int> k) override
	{
		std::vector<float> result(k.size());
		for (size_t idx = 0; idx < k.size(); idx++) {
			result[idx] = amplitude * cos(frequency * 2 * 3.14 * 0.001 * k[idx] + (3.14 * phase / 180));
		}
		return result;
	}

	bool Configure() override
	{
		bool changed = false;
		changed |= ImGui::SliderFloat("Частота", &frequency, 0, 1e+6,
				"%.0f", ImGuiSliderFlags_Logarithmic);
		changed |= ImGui::SliderFloat("Амплитуда", &amplitude, 0, 10);
		changed |= ImGui::SliderFloat("Фаза", &phase, 0, 360);
		return changed;
	}

	void SetEnergy(float E) override
	{
		amplitude = 4 * sqrt(E);
	}

	float GetEnergy() const override
	{
		return amplitude * amplitude / 4;
	}
};

