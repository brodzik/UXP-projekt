#pragma once

#include <istream>
#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <cmath>

using Value = std::variant<int, float, std::string>;


class LindaParser {

    std::istream& inStream;

    int position = -1;
    char currentChar = '\0';

    void generateError(std::string message);
    char getNextChar();
    void expectCharacter(char expectedChar);
    std::optional<char> escapeCharacter(char previousCharacter, 
        char currentCharacter);

    std::optional<Value> buildNumber();
    std::optional<Value> buildString();

public:
    LindaParser(std::istream& inStream): inStream(inStream) {}

    std::vector<Value> buildTuple();
};