#pragma once

#include <vector>

#include <Generator.hpp>
#include <GeneratorNormal.hpp>
#include <GeneratorConst.hpp>
#include <GeneratorHarmonic.hpp>

class SignalGenerator {
private:
	int choosen;
	const int type;

	// Available generators
	// Type 0: Random
	GeneratorNormal gen_0_0;
	// Type 1: Determined
	GeneratorConst gen_1_0;
	GeneratorHarmonic gen_1_1;

	const std::vector<const char*> getNames()
	{
		if (type == 0) {
			return { gen_0_0.Name() };
		} else if (type == 1) {
			return { gen_1_0.Name(), gen_1_1.Name() };
		} else {
			throw;
		}
	}

public:
	SignalGenerator(int type = 0) : choosen{0}, type(type),
		gen_0_0(0, 1),
		gen_1_0(1),
		gen_1_1(1, 2, 0)
	{}

	Generator & GetGenerator()
	{
		switch (type) {
			case 0: // Random signal
				switch (choosen) {
					case 0: return gen_0_0; // Gaussian distribution
				}
				break;
			case 1: // Determined signal
				switch (choosen) {
					case 0: return gen_1_0; // Constant signal
					case 1: return gen_1_1; // Sine wave
				}
				break;
		}
		// Default way - error
		return gen_1_0;
	}

	bool Selector(const char *title)
	{
		const std::vector<const char*> names = getNames();
		return ImGui::Combo(title, &choosen, names.data(), names.size());
	}

	bool Configure()
	{
		return GetGenerator().Configure();
	}

	float Generate(float t)
	{
		return GetGenerator().Generate(t);
	}

	Generator & operator()()
	{
		return GetGenerator();
	}
};

