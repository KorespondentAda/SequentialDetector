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

	virtual std::vector<float> Generate(std::vector<int> ids) = 0;
	virtual std::vector<float> Generate(int n, float start = 0)
	{
		std::vector<int> ids(n);
		for (int i = 0; i < n; i++) {
			ids[i] = i + start;
		}
		return Generate(ids);
	}
};

