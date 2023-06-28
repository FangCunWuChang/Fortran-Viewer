#pragma once
#include <JuceHeader.h>

class DebugWindow final : public juce::DocumentWindow {
public:
	DebugWindow();

	void actionCodeChanged(juce::CodeDocument::Iterator& it, const juce::String& data);

private:
	std::unique_ptr<juce::TextEditor> editor = nullptr;
	std::unique_ptr<juce::CodeTokeniser> tokeniser = nullptr;

	void closeButtonPressed() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugWindow)
};