#pragma once
#include <JuceHeader.h>
#include "MenuBarModel.h"

class MainWindow final : public juce::DocumentWindow {
public:
	MainWindow();
	MainWindow(const juce::String& filePath);
	~MainWindow();

	void actionNew();
	void actionLoad();
	void actionSave() const;

private:
	std::unique_ptr<juce::CodeEditorComponent> editor = nullptr;
	std::unique_ptr<juce::CodeDocument> document = nullptr;
	std::unique_ptr<juce::CodeTokeniser> tokeniser = nullptr;

	std::unique_ptr<MenuBarModel> menuModel = nullptr;

	const juce::StringArray extensions {
		".f90", ".f95", ".f03", ".f08", ".f15"};

	void closeButtonPressed() override;

	bool checkForSave() const;

	bool load(const juce::File& file);
	bool save(const juce::File& file) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};