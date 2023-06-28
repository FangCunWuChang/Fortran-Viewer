#pragma once
#include <JuceHeader.h>
#include "MainWindow.h"
#include "DebugWindow.h"

class ViewerApplication final : public juce::JUCEApplication {
public:
    ViewerApplication() {};

    const juce::String getApplicationName() override { return ProjectInfo::projectName; };
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; };
    bool moreThanOneInstanceAllowed() override { return true; };

    void initialise(const juce::String& commandLine) override {
        this->debugWindow = std::make_unique<DebugWindow>();

        if (commandLine.isEmpty()) {
            this->mainWindow = std::make_unique<MainWindow>(this->debugWindow.get());
        }
        else {
            this->mainWindow = std::make_unique<MainWindow>(commandLine, this->debugWindow.get());
        }

        this->mainWindow->centreWithSize(800, 600);
        this->mainWindow->setVisible(true);

        this->debugWindow->centreWithSize(800, 600);
        this->debugWindow->setVisible(true);
    };

    void shutdown() override {
        this->mainWindow = nullptr;
        this->debugWindow = nullptr;
    };

    void systemRequestedQuit() override {
        quit();
    };

private:
    std::unique_ptr<MainWindow> mainWindow = nullptr;
    std::unique_ptr<DebugWindow> debugWindow = nullptr;
};
