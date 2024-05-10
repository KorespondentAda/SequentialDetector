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

	virtual std::vector<float> Generate(std::vector<float> t) override
	{
		std::vector<float> result(t.size(), amplitude);
		return result;
	}

	virtual bool Configure() override
	{
		return ImGui::SliderFloat("Амплитуда", &amplitude, 0, 10);
	}

	virtual void SetEnergy(float E) override
	{
		amplitude = sqrt(E);
	}

	virtual float GetEnergy() const override
	{
		return amplitude * amplitude;
	}
};

