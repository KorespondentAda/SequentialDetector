#pragma once

#include <imgui.h>
#include <cmath>
#include <vector>
#include <Generator.hpp>

class GeneratorConst final : public Generator {
private:
	float amplitude;

public:
	GeneratorConst(float amplitude) : Generator("Постоянный"),
			amplitude(amplitude)
	{}

	std::vector<float> Generate(std::vector<int> k) override
	{
		std::vector<float> result(k.size(), amplitude);
		return result;
	}

	bool Configure() override
	{
		return ImGui::SliderFloat("Амплитуда", &amplitude, 0, 10);
	}

	void SetEnergy(float E) override
	{
		amplitude = sqrt(E);
	}

	float GetEnergy() const override
	{
		return amplitude * amplitude;
	}
};

