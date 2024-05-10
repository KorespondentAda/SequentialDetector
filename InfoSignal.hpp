#pragma once

#include <cmath>
#include <imgui.h>
#include <implot.h>
#include <WindowControlled.hpp>
#include <SignalGenerator.hpp>

class InfoSignal : public WindowControlled {
private:
	SignalGenerator gen;
	const float time_step;

	virtual void Setup() override
	{
		ImGui::TextWrapped("Установка вида обнаруживаемого сигнала и его параметров");
		gen.Selector("Вид сигнала");
		gen.Configure();
	}

public:
	InfoSignal(float tstep = 0.001) :
			WindowControlled("Обнаруживаемый сигнал"),
			gen(1),
			time_step(tstep)
	{}

	Generator & operator()()
	{
		return gen();
	}
};

