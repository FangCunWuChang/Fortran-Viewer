#include "DebugWindow.h"
#include "FortranTokeniser.h"

DebugWindow::DebugWindow()
	: DocumentWindow(juce::String(ProjectInfo::projectName) + " Debug",
		juce::Desktop::getInstance().getDefaultLookAndFeel()
		.findColour(juce::ResizableWindow::backgroundColourId),
		juce::DocumentWindow::TitleBarButtons::allButtons) {
	this->setUsingNativeTitleBar(true);
	this->setResizable(true, false);

	this->tokeniser = std::unique_ptr<juce::CodeTokeniser>(new FortranTokeniser);

	this->editor = std::make_unique<juce::TextEditor>();
	this->editor->setMultiLine(true);
	this->editor->setReadOnly(true);
	this->setContentNonOwned(this->editor.get(), false);
}

void DebugWindow::closeButtonPressed() {
	this->setVisible(false);
}

void DebugWindow::actionCodeChanged(juce::CodeDocument::Iterator& it, const juce::String& data) {
	this->editor->clear();
	this->editor->setReadOnly(false);

	juce::Array<std::tuple<int, int>> tokenList;
	juce::CodeDocument::Iterator lastIterator = it;
	const int lineLength = data.length();
	for (;;) {
		int tokenType = this->tokeniser->readNextToken(it);
		int tokenStart = lastIterator.getPosition();
		int tokenEnd = it.getPosition();

		if (tokenEnd <= tokenStart)
			break;

		if (tokenEnd > 0) {
			const int start = juce::jmax(0, tokenStart);
			tokenList.add({ start, tokenType });

			if (tokenEnd >= lineLength)
				break;
		}

		lastIterator = it;
	}
	/*while (!it.isEOF()) {
		int pos = it.getPosition();
		int result = this->tokeniser->readNextToken(it);
		tokenList.add({ pos, result });
	}*/

	std::map<juce::String, int> idMap;
	juce::Array<std::tuple<juce::String, juce::String>> binaryList;
	for (int i = 0; i < tokenList.size(); i++) {
		auto& [pos, type] = tokenList.getReference(i);
		int posFinish = pos;
		if (i == tokenList.size() - 1) {
			posFinish = data.length();
		}
		else {
			std::tie(posFinish, std::ignore) = tokenList.getReference(i + 1);
		}

		juce::String token = data.substring(pos, posFinish);
		token = token.removeCharacters("\r\n\v\f");
		token = token.trim();

		switch (type) {
		case FortranTokeniser::TokenType::tokenType_identifier:
		case FortranTokeniser::TokenType::tokenType_keyword: {
			auto it = idMap.find(token);
			if (it != idMap.end()) {
				binaryList.add({ token, juce::String(it->second) });
			}
			else {
				idMap.insert({ token, pos });
				binaryList.add({token, juce::String(pos) });
			}
			break;
		}
		
		case FortranTokeniser::TokenType::tokenType_string:
		case FortranTokeniser::TokenType::tokenType_float:
		case FortranTokeniser::TokenType::tokenType_integer: {
			auto it = idMap.find(token);
			if (it != idMap.end()) {
				binaryList.add({ token, juce::String(++(it->second)) });
			}
			else {
				idMap.insert({ token, 1 });
				binaryList.add({ token, juce::String(1) });
			}
			break;
		}

		default:
			binaryList.add({ token, "_"});
			break;
		}
	}

	for (auto& i : binaryList) {
		auto& [token, attr] = i;
		this->editor->moveCaretToEnd();
		this->editor->insertTextAtCaret("(" + token + "," + attr + ")\n");
	}

	this->editor->setReadOnly(true);
}
