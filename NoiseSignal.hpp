#pragma once

#include <imgui.h>
#include <WindowControlled.hpp>

class NoiseSignal : public WindowControlled {
private:
	SignalGenerator gen;

	virtual void Setup() override
	{
		ImGui::TextWrapped("Установка вида шумового фона обнаруживаемого сигнала");
		gen.Selector("Вид распределения шума");
		gen.Configure();
	}

public:
	NoiseSignal() :
			WindowControlled("Шумовой сигнал"),
			gen(0)
	{}

	Generator & operator()()
	{
		return gen();
	}
};

