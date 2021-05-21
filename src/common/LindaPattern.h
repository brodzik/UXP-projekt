#pragma once

#include <sstream>

#include "LindaParser.h"

class LindaPattern {


public:
    std::vector<PatternEntry> entries;

    LindaPattern(std::string textToParse) {
        std::stringstream inStream(textToParse);
        LindaParser lindaParser(inStream);
        entries = lindaParser.buildPattern();
    }

    LindaPattern(std::vector<PatternEntry> entries): entries(entries) {}
};