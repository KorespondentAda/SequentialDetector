#pragma once

#include <imgui.h>
#include <WindowControlled.hpp>

class NoiseSignal : public WindowControlled {
private:
	SignalGenerator gen;
	const float time_step;

	virtual void Setup() override
	{
		ImGui::TextWrapped("Установка вида шумового фона обнаруживаемого сигнала");
		gen.Selector("Вид сигнала");
		gen.Configure();
	}

public:
	NoiseSignal(float tstep = 0.001) :
			WindowControlled("Шумовой сигнал"),
			gen(0),
			time_step(tstep)
	{}

	Generator & operator()()
	{
		return gen.GetGenerator();
	}
};

