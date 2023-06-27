#include "FortranTokeniser.h"

struct FortranTokeniserFunctions final {
	static bool isReservedKeyword(juce::String::CharPointerType token, const int tokenLength) noexcept {
		static const char* const keywords2Char[] = {
			"do", "go", "to", "if", "in", "or", "eq", "ne", "gt", "lt", "ge", "le", nullptr};

		static const char* const keywords3Char[] = {
			"end", "len", "out", "use", "and", "eqv", "not", nullptr};

		static const char* const keywords4Char[] = {
			"data", "call", "case", "else", "exit", "kind", "only", "real", "save",
			"stop", "then", "type", "open", "read", "true", "neqv", "none", nullptr};

		static const char* const keywords5Char[] = {
			"block", "cycle", "entry", "inout", "pause", "where", "while", "close",
			"print", "write", "false", nullptr};

		static const char* const keywords6Char[] = {
			"assign", "common", "double", "intent", "module", "public", "result",
			"return", "select", "target", "format", "rewind", nullptr };

		static const char* const keywords7Char[] = {
			"complex", "default", "integer", "logical", "nullify", "pointer",
			"private", "program", "endfile", "inquire", nullptr };

		static const char* const keywords8Char[] = {
			"allocate", "contains", "continue", "external", "function", "implicit",
			"namelist", "operator", "optional", nullptr };

		static const char* const keywords9Char[] = {
			"character", "precision", "elsewhere", "interface", "intrinsic",
			"parameter", "recursive", "backspace", nullptr };

		static const char* const keywordsOther[] = {
			"assignment", "deallocate", "subroutine", "allocatable", "equivalence", nullptr };

		const char* const* k;

		switch (tokenLength) {
		case 2:   k = keywords2Char; break;
		case 3:   k = keywords3Char; break;
		case 4:   k = keywords4Char; break;
		case 5:   k = keywords5Char; break;
		case 6:   k = keywords6Char; break;
		case 7:   k = keywords7Char; break;
		case 8:   k = keywords8Char; break;
		case 9:   k = keywords9Char; break;

		default:
			if (tokenLength < 2 || tokenLength > 16)
				return false;

			k = keywordsOther;
			break;
		}

		for (int i = 0; k[i] != nullptr; ++i)
			if (token.compare(juce::CharPointer_ASCII(k[i])) == 0)
				return true;

		return false;
	}

	template <typename Iterator>
	static int parseIdentifier(Iterator& source) noexcept {
		int tokenLength = 0;
		juce::String::CharPointerType::CharType possibleIdentifier[100] = {};
		juce::String::CharPointerType possible(possibleIdentifier);

		while (juce::CppTokeniserFunctions::isIdentifierBody(source.peekNextChar())) {
			auto c = source.nextChar();

			if (tokenLength < 32)
				possible.write(c);

			++tokenLength;
		}

		if (tokenLength > 1 && tokenLength <= 16) {
			possible.writeNull();

			if (isReservedKeyword(juce::String::CharPointerType(possibleIdentifier), tokenLength))
				return FortranTokeniser::tokenType_keyword;
		}

		return FortranTokeniser::tokenType_identifier;
	}

	template <typename Iterator>
	static int readNextToken(Iterator& source) {
		source.skipWhitespace();

		auto firstChar = source.peekNextChar();

		switch (firstChar) {
		case 0:
			break;

		case '0':   case '1':   case '2':   case '3':   case '4':
		case '5':   case '6':   case '7':   case '8':   case '9':
		case '.': {
			auto result = juce::CppTokeniserFunctions::parseNumber(source);

			if (result == FortranTokeniser::tokenType_error) {
				source.skip();

				if (firstChar == '.')
					return FortranTokeniser::tokenType_punctuation;
			}

			return result;
		}

		case ',':
		case ';':
		case '&':
			source.skip();
			return FortranTokeniser::tokenType_punctuation;

		case ':':
			source.skip();
			juce::CppTokeniserFunctions::skipIfNextCharMatches(source, ':');
			return FortranTokeniser::tokenType_punctuation;

		case '(':
			source.skip();
			juce::CppTokeniserFunctions::skipIfNextCharMatches(source, '/');
			return FortranTokeniser::tokenType_bracket;

		case ')':
		case '{':   case '}':
		case '[':   case ']':
			source.skip();
			return FortranTokeniser::tokenType_bracket;

		case '"':
		case '\'':
			juce::CppTokeniserFunctions::skipQuotedString(source);
			return FortranTokeniser::tokenType_string;

		case '+':
			source.skip();
			return FortranTokeniser::tokenType_operator;

		case '-': {
			source.skip();
			auto result = juce::CppTokeniserFunctions::parseNumber(source);

			if (result == FortranTokeniser::tokenType_error) {
				return FortranTokeniser::tokenType_operator;
			}

			return result;
		}

		case '!':
			source.skipToEndOfLine();
			return FortranTokeniser::tokenType_comment;

		case '*':
			source.skip();
			juce::CppTokeniserFunctions::skipIfNextCharMatches(source, '*');
			return FortranTokeniser::tokenType_operator;

		case '=':
			source.skip();
			juce::CppTokeniserFunctions::skipIfNextCharMatches(source, '=');
			return FortranTokeniser::tokenType_operator;

		case '/':
			source.skip();

			if (source.peekNextChar() == ')') {
				source.skip();
				return FortranTokeniser::tokenType_bracket;
			}

			juce::CppTokeniserFunctions::skipIfNextCharMatches(source, '=');
			return FortranTokeniser::tokenType_operator;

		case '<':   case '>':
			source.skip();
			juce::CppTokeniserFunctions::skipIfNextCharMatches(source, '=');
			return FortranTokeniser::tokenType_operator;

		default:
			if (juce::CppTokeniserFunctions::isIdentifierStart(firstChar))
				return parseIdentifier(source);

			source.skip();
			break;
		}

		return FortranTokeniser::tokenType_error;
	}
};

FortranTokeniser::FortranTokeniser() {}
FortranTokeniser::~FortranTokeniser() {}

int FortranTokeniser::readNextToken(juce::CodeDocument::Iterator& source) {
	return FortranTokeniserFunctions::readNextToken(source);
}

juce::CodeEditorComponent::ColourScheme FortranTokeniser::getDefaultColourScheme() {
	juce::CodeEditorComponent::ColourScheme cs;

	cs.set("Error", juce::Colour{0xffe60000});
	cs.set("Comment", juce::Colour{0xff72d20c});
	cs.set("Keyword", juce::Colour{0xffee6f6f});
	cs.set("Operator", juce::Colour{0xffc4eb19});
	cs.set("Identifier", juce::Colour{0xffcfcfcf});
	cs.set("Integer", juce::Colour{0xff42c8c4});
	cs.set("Float", juce::Colour{0xff885500});
	cs.set("String", juce::Colour{0xffbc45dd});
	cs.set("Bracket", juce::Colour{0xff058202});
	cs.set("Punctuation", juce::Colour{0xffcfbeff});

	return cs;
}
