#pragma once

#include <vector>

class Generator {
private:
	const char * name;

public:
	Generator(const char * name) : name(name)
	{}
	const char * Name() const { return name; }

	virtual bool Configure() = 0;

	virtual void SetEnergy(float E) = 0;
	virtual float GetEnergy() const = 0;

	virtual std::vector<float> Generate(std::vector<float> time) = 0;
	virtual float Generate(float t)
	{
		return Generate(std::vector<float>(1, t))[0];
	}
	virtual std::vector<float> Generate(int n, float ts, float tstart = 0)
	{
		std::vector<float> times(n);
		for (int i = 0; i < n; i++) {
			times[i] = i * ts + tstart;
		}
		return Generate(times);
	}
};

