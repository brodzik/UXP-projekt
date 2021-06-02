#pragma once

#include <sstream>

#include "LindaParser.h"
#include "LindaTuple.h"

class LindaPattern {

    bool checkTypes(Value tupleValue, int entryIndex) {
        if(std::get_if<int>(&tupleValue) && 
            Type::INT == entries[entryIndex].type)
            return true;
        if(std::get_if<float>(&tupleValue) && 
            Type::FLOAT == entries[entryIndex].type)
            return true;
        if(std::get_if<std::string>(&tupleValue) && 
            Type::STRING == entries[entryIndex].type)
            return true;
        return false;
    }

    bool isMatchingPatternEntry(Value tupleValue, int entryIndex) {
        switch (entries[entryIndex].condition)
        {
        case Condition::ANY:
            return true;
        case Condition::EQUAL:
            return tupleValue   ==    entries[entryIndex].value.value();
        case Condition::LESS_EQUAL:
            return tupleValue   <=    entries[entryIndex].value.value();
        case Condition::LESS_THAN:
            return tupleValue   <     entries[entryIndex].value.value();
        case Condition::MORE_EQUAL:
            return tupleValue   >=    entries[entryIndex].value.value();
        case Condition::MORE_THAN:
            return tupleValue   >     entries[entryIndex].value.value();
        default:
            return false;
        }

    }

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

    bool isMatching(LindaTuple lindaTuple) {
        unsigned int tupleIndex = 0;
        for(unsigned int patternIndex = 0; patternIndex < entries.size(); 
            ++patternIndex) {
            
            // tuple out of bounds
            if(tupleIndex >= lindaTuple.values.size()) {
                if(Condition::ANY != entries[patternIndex].condition)
                    return false;
                continue;
            }
            
            // type mismatch
            if(!checkTypes(lindaTuple.values[tupleIndex], patternIndex)) {
                if(Condition::ANY == entries[patternIndex].condition)
                    continue;
                return false;
            }
            
            // normal comparison
            if(!isMatchingPatternEntry(lindaTuple.values[tupleIndex], 
                patternIndex))
                return false;

            ++tupleIndex;
        }
        // whole tuple processed
        if(tupleIndex < lindaTuple.values.size())
            return false;
        
        // everything ok
        return true;
    }

};
