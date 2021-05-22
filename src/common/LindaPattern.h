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

    std::string toString() {
        std::string toReturnString = "(";
        for(PatternEntry entry: entries) {
            toReturnString += printType(entry.type) + ":";
            toReturnString += printCondition(entry.condition);
            if(entry.value)
                toReturnString += printValue(entry.value.value());
            toReturnString += ",";
        }

        toReturnString.pop_back();
        toReturnString += ")";
        return toReturnString;
    }
};