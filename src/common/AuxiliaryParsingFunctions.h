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
