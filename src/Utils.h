#pragma once
#include <JuceHeader.h>

namespace utils {
	class DefaultDir final : private juce::DeletedAtShutdown {
	public:
		static juce::File get();
		static void set(const juce::File& dir);

	private:
		DefaultDir();
		~DefaultDir();

		juce::File dir;

		JUCE_DECLARE_SINGLETON(DefaultDir, false);
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultDir)
	};
}
