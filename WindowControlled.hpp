#pragma once

class WindowControlled {
private:
	bool windowOpened;
	const char * winTitle;
	virtual void Setup() = 0;

public:
	WindowControlled(const char * WinTitle) : winTitle(WinTitle)
	{}

	void Show(const char *title, bool inl = false)
	{
		if (inl) {
			Setup();
		} else {
			if (ImGui::Button(title)) windowOpened = !windowOpened;
			if (windowOpened) {
				ImGui::Begin(winTitle, &windowOpened);
				Setup();
				ImGui::End();
			}
		}
	}
};
