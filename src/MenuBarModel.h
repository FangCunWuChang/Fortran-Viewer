#pragma once
#include <JuceHeader.h>

class MainWindow;

class MenuBarModel final : public juce::MenuBarModel {
public:
	MenuBarModel() = delete;
	explicit MenuBarModel(MainWindow* parent);

	juce::StringArray getMenuBarNames() override {
		return { "File" };
	};

	juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override {
		switch (topLevelMenuIndex) {
		case 0: {
			juce::PopupMenu menu;
			menu.addItem(1, "New");
			menu.addItem(2, "Load");
			menu.addItem(3, "Save");
			return menu;
		}
		default:
			break;
		}
		return juce::PopupMenu{};
	};

	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
	MainWindow* parent = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBarModel)
};