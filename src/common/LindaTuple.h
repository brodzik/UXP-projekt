#pragma once

#include <sstream>

#include "LindaParser.h"

class LindaTuple {
public:
    std::vector<Value> values;
    LindaTuple() = default;
    LindaTuple(std::string textToParse) {
        std::stringstream inStream(textToParse);
        LindaParser lindaParser(inStream);
        values = lindaParser.buildTuple();
    }
};