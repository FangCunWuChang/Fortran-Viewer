#pragma once
#include <JuceHeader.h>
#include "MainWindow.h"

class ViewerApplication final : public juce::JUCEApplication {
public:
    ViewerApplication() {};

    const juce::String getApplicationName() override { return ProjectInfo::projectName; };
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; };
    bool moreThanOneInstanceAllowed() override { return true; };

    void initialise(const juce::String& commandLine) override {
        if (commandLine.isEmpty()) {
            this->mainWindow = std::make_unique<MainWindow>();
        }
        else {
            this->mainWindow = std::make_unique<MainWindow>(commandLine);
        }

        this->mainWindow->centreWithSize(800, 600);
        this->mainWindow->setVisible(true);
    };

    void shutdown() override {
        this->mainWindow = nullptr;
    };

    void systemRequestedQuit() override {
        quit();
    };

private:
    std::unique_ptr<MainWindow> mainWindow = nullptr;
};
