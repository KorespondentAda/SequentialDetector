#pragma once

#include <imgui.h>
#include <cmath>
#include <Generator.hpp>
#include <random>

class GeneratorNormal : public Generator {
private:
	float μ;
	float σ;
	std::mt19937 gen;
	std::normal_distribution<float> randn;

public:
	GeneratorNormal(float mean, float stddev) : Generator("Гауссовский"),
			μ(mean), σ(stddev)
	{
#ifndef DEBUG
		std::random_device rd{};
		gen{rd()};
#else
		gen = std::mt19937{};
#endif
	}

	virtual std::vector<float> Generate(std::vector<float> t) override
	{
		std::vector<float> result(t);
		for (auto &el : result) {
			el = randn(gen);
		}
		return result;
	}

	virtual bool Configure() override
	{
		bool changed = false;
		changed |= ImGui::SliderFloat("Среднее", &μ, -5, 5);
		changed |= ImGui::SliderFloat("СКО", &σ, 0, 10);
		if (changed) {
			randn.param(std::normal_distribution<float>::param_type(μ, σ));
		}
		return changed;
	}

	virtual void SetEnergy(float E) override
	{
		σ = sqrt(E);
	}

	virtual float GetEnergy() const override
	{
		return σ * σ;
	}
};
