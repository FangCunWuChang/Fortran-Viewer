#include "MenuBarModel.h"
#include "MainWindow.h"

MenuBarModel::MenuBarModel(MainWindow* parent)
	: parent(parent) {}

void MenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex) {
	if (topLevelMenuIndex == 0) {
		switch (menuItemID) {
		case 1:
			juce::MessageManager::callAsync([parent = this->parent] {parent->actionNew(); });
			break;
		case 2:
			juce::MessageManager::callAsync([parent = this->parent] {parent->actionLoad(); });
			break;
		case 3:
			juce::MessageManager::callAsync([parent = this->parent] {parent->actionSave(); });
			break;
		default:
			break;
		}
	}
}
