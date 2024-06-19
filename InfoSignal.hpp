#pragma once

#include <cmath>
#include <imgui.h>
#include <implot.h>
#include <WindowControlled.hpp>
#include <SignalGenerator.hpp>

class InfoSignal : public WindowControlled {
private:
	SignalGenerator gen;

	virtual void Setup() override
	{
		ImGui::TextWrapped("Установка вида обнаруживаемого сигнала и его параметров");
		gen.Selector("Вид сигнала");
		gen.Configure();
	}

public:
	InfoSignal() :
			WindowControlled("Обнаруживаемый сигнал"),
			gen(1)
	{}

	Generator & operator()()
	{
		return gen();
	}
};

