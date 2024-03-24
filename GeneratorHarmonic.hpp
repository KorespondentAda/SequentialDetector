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

	virtual std::vector<float> Generate(std::vector<float> t) override
	{
		std::vector<float> result(t);
		auto l = std::transform(t.cbegin(), t.cend(), result.begin(), [this](float t){
				return amplitude * cos(frequency * 2 * 3.14 * t + (3.14 * phase / 180));
		});
		return result;
	}

	virtual bool Configure() override
	{
		bool changed = false;
		changed |= ImGui::SliderFloat("Частота", &frequency, 0, 1e+6,
				"%.0f", ImGuiSliderFlags_Logarithmic);
		changed |= ImGui::SliderFloat("Амплитуда", &amplitude, 0, 10);
		changed |= ImGui::SliderFloat("Фаза", &phase, 0, 360);
		return changed;
	}

	virtual void SetEnergy(float E) override
	{
		amplitude = 4 * sqrt(E);
	}

	virtual float GetEnergy() const override
	{
		return amplitude * amplitude / 4;
	}
};

