#pragma once

#include <istream>
#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <cmath>

#include "AuxiliaryParsingFunctions.h"

class LindaParser {

    std::istream& inStream;

    int position = -1;
    char currentChar = '\0';

    void generateError(std::string message);
    char getNextChar();
    void skipWhites();
    void expectCharacter(char expectedChar);
    bool checkNextCharacter(char expectedChar);
    std::optional<char> escapeCharacter(char previousCharacter, 
        char currentCharacter);
    void checkTypeAndValue(Type type, Value value);

    std::optional<Value> buildNumber();
    std::optional<Value> buildString();
    std::optional<Type> buildType();
    Condition buildCondition();

public:
    LindaParser(std::istream& inStream): inStream(inStream) {}

    std::vector<Value> buildTuple();
    std::vector<PatternEntry> buildPattern();
};