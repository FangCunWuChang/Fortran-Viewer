#include "MainWindow.h"
#include "FortranTokeniser.h"
#include "Utils.h"

MainWindow::MainWindow() 
	: DocumentWindow(ProjectInfo::projectName,
		juce::Desktop::getInstance().getDefaultLookAndFeel()
		.findColour(juce::ResizableWindow::backgroundColourId),
		juce::DocumentWindow::TitleBarButtons::allButtons) {
	this->setUsingNativeTitleBar(true);
	this->setResizable(true, false);

	/** Code Editor */
	this->document = std::make_unique<juce::CodeDocument>();
	this->document->setSavePoint();

	this->tokeniser = std::unique_ptr<juce::CodeTokeniser>(new FortranTokeniser);
	this->editor = std::make_unique<juce::CodeEditorComponent>(
		*(this->document.get()), this->tokeniser.get());
	this->setContentNonOwned(this->editor.get(), false);

	/** Menu Bar */
	this->menuModel = std::make_unique<MenuBarModel>(this);
	this->setMenuBar(this->menuModel.get());
}

MainWindow::MainWindow(const juce::String& filePath)
	: MainWindow() {
	juce::File file = juce::File::getCurrentWorkingDirectory().getChildFile(filePath);
	utils::DefaultDir::set(file.getParentDirectory());

	if (!this->load(file)) {
		juce::AlertWindow::showMessageBox(
			juce::AlertWindow::WarningIcon, ProjectInfo::projectName,
			"Failed to load file : " + file.getFullPathName());
	}
}

MainWindow::~MainWindow() {
	this->setContentNonOwned(nullptr, false);
	this->editor = nullptr;
	this->setMenuBar(nullptr);
}

void MainWindow::actionNew() {
	if (!this->checkForSave()) { return; }

	this->document->deleteSection(0, this->document->getNumCharacters());
	this->document->clearUndoHistory();
	this->document->setSavePoint();
}

void MainWindow::actionLoad() {
	if (!this->checkForSave()) { return; }

	juce::FileChooser fileChooser{
		ProjectInfo::projectName, utils::DefaultDir::get(),
			"*" + this->extensions.joinIntoString(";*")};
	if (!fileChooser.browseForFileToOpen()) {
		return;
	}

	juce::File file = fileChooser.getResult();
	utils::DefaultDir::set(file.getParentDirectory());
	if (!this->load(file)) {
		juce::AlertWindow::showMessageBox(
			juce::AlertWindow::WarningIcon, ProjectInfo::projectName,
			"Failed to load file : " + file.getFullPathName());
	}
}

void MainWindow::actionSave() const {
	juce::FileChooser fileChooser{
		ProjectInfo::projectName, utils::DefaultDir::get(),
			"*" + this->extensions.joinIntoString(";*")};
	if (!fileChooser.browseForFileToSave(true)) {
		return;
	}

	juce::File file = fileChooser.getResult();
	utils::DefaultDir::set(file.getParentDirectory());
	if (!this->save(file)) {
		juce::AlertWindow::showMessageBox(
			juce::AlertWindow::WarningIcon, ProjectInfo::projectName,
			"Failed to save file : " + file.getFullPathName());
	}
}

void MainWindow::closeButtonPressed() {
	if (this->checkForSave()) {
		juce::JUCEApplication::quit();
	}
}

bool MainWindow::checkForSave() const {
	if (this->document->hasChangedSinceSavePoint()) {
		int result = juce::AlertWindow::showYesNoCancelBox(
			juce::AlertWindow::QuestionIcon, ProjectInfo::projectName,
			"Document has changed, do you want to save?");
		if (result == 0) {
			return false;
		}
		if (result == 2) {
			return true;
		}

		juce::FileChooser fileChooser{
			ProjectInfo::projectName, utils::DefaultDir::get(),
			"*" + this->extensions.joinIntoString(";*")};
		if (!fileChooser.browseForFileToSave(true)) {
			return false;
		}

		juce::File file = fileChooser.getResult();
		utils::DefaultDir::set(file.getParentDirectory());
		if (!this->save(file)) {
			juce::AlertWindow::showMessageBox(
				juce::AlertWindow::WarningIcon, ProjectInfo::projectName,
				"Failed to save file : " + file.getFullPathName());
			return false;
		}
	}
	return true;
}

bool MainWindow::load(const juce::File& file) {
	if (!this->extensions.contains(file.getFileExtension())) {
		return false;
	}
	if (!file.existsAsFile()) {
		return false;
	}

	juce::FileInputStream stream{file};
	if (stream.failedToOpen()) {
		return false;
	}
	if (!this->document->loadFromStream(stream)) {
		return false;
	}

	this->document->setSavePoint();
	return true;
}

bool MainWindow::save(const juce::File& file) const {
	if (!this->extensions.contains(file.getFileExtension())) {
		return false;
	}

	juce::FileOutputStream stream{file};
	if (stream.failedToOpen()) {
		return false;
	}
	stream.setPosition(0);
	stream.truncate();
	if (!this->document->writeToStream(stream)) {
		return false;
	}

	this->document->setSavePoint();
	return true;
}
