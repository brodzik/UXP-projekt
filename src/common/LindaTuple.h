#pragma once

#include <sstream>

#include "LindaParser.h"

class LindaTuple {

    std::string addEscaping(std::string text) { 
        std::string toReturnString;
        for(unsigned int i = 0; i < text.size(); ++i) {
            if('\\' == text[i] || '\"' == text[i])
                toReturnString += '\\';
            toReturnString += text[i];
        }
        return toReturnString;
    }

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
        for(Value value: values) {
            
            if(const int* val = std::get_if<int>(&value)) {
                toReturnString += std::to_string(*val) + ",";
                continue;
            }

            if(const float* val = std::get_if<float>(&value)) {
                toReturnString += std::to_string(*val) + ",";
                continue;
            }

            if(const std::string* val = std::get_if<std::string>(&value)) {
                toReturnString += "\"" + addEscaping(*val) + "\"" + ",";
                continue;
            }
        }

        toReturnString.pop_back();
        toReturnString += ")";
        return toReturnString;
    }
};