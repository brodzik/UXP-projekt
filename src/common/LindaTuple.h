#pragma once

#include <sstream>

#include "LindaParser.h"

class LindaTuple {


public:
    std::vector<Value> values;

    LindaTuple(std::string textToParse) {
        std::stringstream inStream(textToParse);
        LindaParser lindaParser(inStream);
        values = lindaParser.buildTuple();
    }

    LindaTuple(std::vector<Value> values): values(values) {}

    std::string toString() {
        std::string toReturnString = "(";
        for(Value value: values)
            toReturnString += printValue(value) + ",";

        toReturnString.pop_back();
        toReturnString += ")";
        return toReturnString;
    }
};
