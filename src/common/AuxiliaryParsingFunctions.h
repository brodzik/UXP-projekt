#pragma once

#include <variant>
#include <string>
#include <optional>

using Value = std::variant<int, float, std::string>;


enum Type {
    INT,
    FLOAT,
    STRING
};

enum Condition {
    EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    MORE_THAN,
    MORE_EQUAL,
    ANY
};

class PatternEntry {
public:
    Type type;
    Condition condition;
    std::optional<Value> value;
    PatternEntry(Type type, Condition condition, std::optional<Value> value): 
        type(type), condition(condition), value(value) {}
};




inline std::string printType(Type type) {
    switch (type)
    {
    case Type::INT:
        return "int";
    case Type::FLOAT:
        return "float";
    case Type::STRING:
        return "string";
    default:
        return "Incorrect type";
    }
}

inline std::string printCondition(Condition condition) {
    switch (condition)
    {
    case Condition::ANY:
        return "*";
    case Condition::EQUAL:
        return "";
    case Condition::LESS_EQUAL:
        return "<=";
    case Condition::LESS_THAN:
        return "<";
    case Condition::MORE_EQUAL:
        return ">=";
    case Condition::MORE_THAN:
        return ">";
    default:
        return "Incorrect condition";
    }
}

inline std::string addEscaping(std::string text) { 
    std::string toReturnString;
    for(unsigned int i = 0; i < text.size(); ++i) {
        if('\\' == text[i] || '\"' == text[i])
            toReturnString += '\\';
        toReturnString += text[i];
    }
    return toReturnString;
}

inline std::string printValue(Value value) {
    if(const int* val = std::get_if<int>(&value))
        return std::to_string(*val);

    if(const float* val = std::get_if<float>(&value))
        return std::to_string(*val);

    if(const std::string* val = std::get_if<std::string>(&value))
        return "\"" + addEscaping(*val) + "\"";
    
    return "Value of incorrect type";
}
