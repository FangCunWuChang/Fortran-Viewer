#include "Utils.h"

namespace utils {
	JUCE_IMPLEMENT_SINGLETON(DefaultDir)

	DefaultDir::DefaultDir()
		: dir(juce::File::getCurrentWorkingDirectory()) {}

	DefaultDir::~DefaultDir() {
		this->clearSingletonInstance();
	}

	juce::File DefaultDir::get() {
		return DefaultDir::getInstance()->dir;
	}

	void DefaultDir::set(const juce::File& dir) {
		DefaultDir::getInstance()->dir = dir;
	}
}