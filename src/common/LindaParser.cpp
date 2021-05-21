#include "LindaParser.h"



void LindaParser::generateError(std::string message) {
    throw std::string("Position: " + std::to_string(position) + " -> " + 
        message);
}


char LindaParser::getNextChar() {
    do {
        currentChar = inStream.get();
        ++position;
    } while(isspace(currentChar));

    return currentChar;
}



void LindaParser::expectCharacter(char expectedChar) {
    if(currentChar != expectedChar)
        generateError("Expected " + expectedChar + std::string(", but got") 
            + currentChar);
    getNextChar();
}


bool LindaParser::checkNextCharacter(char expectedChar) {
    if(currentChar != expectedChar)
        return false;
    getNextChar();
    return true;
}



// converts current char into an escaped character based on previous character
// used in string parsing
std::optional<char> LindaParser::escapeCharacter(char previousCharacter, 
    char currentCharacter) {
    if('\\' != previousCharacter)
        return std::nullopt;
    
    if('\\' == currentCharacter)
        return '\\';
    if('\"' == currentCharacter)
        return '\"';

    return std::nullopt;
}



void LindaParser::checkTypeAndValue(Type type, Value value) {
    if(type == Type::INT) {
        if(!std::get_if<int>(&value))
            generateError("Expected value to be of type INT");
        return;
    }

    if(type == Type::FLOAT) {
        if(!std::get_if<float>(&value))
            generateError("Expected value to be of type FLOAT");
        return;
    }

    if(type == Type::STRING) {
        if(!std::get_if<std::string>(&value))
            generateError("Expected value to be of type STRING");
        return;
    }

}



    

std::optional<Value> LindaParser::buildNumber() {
    if(!isdigit(currentChar))
        return std::nullopt;
    
    int integerPart = currentChar - '0';
    getNextChar();
    
    if(isdigit(currentChar) && 0 == integerPart)
        generateError("Leading zeros not allowed");
    
    while(isdigit(currentChar)) {
        if((std::numeric_limits<int>::max() - (currentChar - '0')) / 10 
            < integerPart)
            generateError("Number too big");
        integerPart = integerPart * 10 + currentChar - '0';
        getNextChar();
    }
    
    if('.' != currentChar)
        return integerPart;
    
    //float part
    int floatingPointPart = 0;
    int decimalPlaces = 0;
    bool isOverflown = false;
    getNextChar();
    while(isdigit(currentChar)) {
        
        if((std::numeric_limits<int>::max() - (currentChar - '0')) / 10 
            < floatingPointPart)
            isOverflown = true;
        
        if(!isOverflown) {
            floatingPointPart = floatingPointPart * 10 + currentChar - '0';
            ++decimalPlaces;
        }
        getNextChar();
    }
    float fullNumber = static_cast<float>(integerPart) 
        + static_cast<float>(floatingPointPart) 
        / pow(10, static_cast<float>(decimalPlaces));
    return fullNumber;
}




std::optional<Value> LindaParser::buildString() {
    std::string text;
    bool isLastCharacterEscaped = false;
    
    if('\"' != currentChar)
        return std::nullopt;

    getNextChar();

    // process first char - this is separate from the main loop to simplify
    // escaping code
    if(-1 == currentChar)
        generateError("Unexpected end of text");

    if('\"' == currentChar) {
        getNextChar();
        return std::string("");
    }

    text += currentChar;
    getNextChar();

    while(true) {
        if(-1 == currentChar)
            generateError("Unexpected end of text");

        //if unescaped quote then end of string
        if('\"' == currentChar && 
            ('\\' != text.back() || isLastCharacterEscaped))
                break;

        std::optional<char> tempChar = escapeCharacter(text.back(), 
            currentChar);
        if(tempChar) {
            text.pop_back();
            currentChar = tempChar.value();
            isLastCharacterEscaped = true;
        }
        else
            isLastCharacterEscaped = false;

        text += currentChar;
        getNextChar();
    }
    
    getNextChar();
    return text;
}




std::optional<Type> LindaParser::buildType() {
    std::string identifierString;
    if(!isalpha(currentChar))
        return std::nullopt;

    do {
        identifierString += currentChar;
        getNextChar();
    } while(isalpha(currentChar));

    if("int"         == identifierString)    return Type::INT;
    if("float"       == identifierString)    return Type::FLOAT;
    if("string"      == identifierString)    return Type::STRING;
    
    return std::nullopt;
}



Condition LindaParser::buildCondition() {
    
    if(checkNextCharacter('*'))
        return Condition::ANY;
    
    if(checkNextCharacter('<')) {
        if(checkNextCharacter('='))
            return Condition::LESS_EQUAL;
        return Condition::LESS_THAN;
    }

    if(checkNextCharacter('>')) {
        if(checkNextCharacter('='))
            return Condition::MORE_EQUAL;
        return Condition::MORE_THAN;
    }

    return Condition::EQUAL;
}




std::vector<Value> LindaParser::buildTuple() {
    std::vector<Value> values;
    bool isProcessed = false;
    
    getNextChar();
    expectCharacter('(');
    while(!isProcessed) {
        std::optional<Value> value = buildNumber();
        if(!value) 
            value = buildString();
        
        if(!value)
            generateError("Expected number or string");
        
        values.push_back(value.value());
        if(checkNextCharacter(')'))
            isProcessed = true;
        else
            expectCharacter(',');
    }
    expectCharacter(-1); // EOT
 
    return values;
}


std::vector<PatternEntry> LindaParser::buildPattern() {
    std::vector<PatternEntry> patternEntries;
    bool isProcessed = false;

    getNextChar();
    expectCharacter('(');
    while(!isProcessed) {
        std::optional<Type> type = buildType();
        if(!type)
            generateError("Expected type");
        
        expectCharacter(':');

        Condition condition = buildCondition();

        std::optional<Value> value = std::nullopt;

        if(FLOAT == type && EQUAL == condition)
            generateError("Condition EQUAL for type FLOAT is illegal");

        if(ANY != condition) {
            value = buildNumber();
            if(!value) 
                value = buildString();
            if(!value)
                generateError("Expected number or string");
            
            checkTypeAndValue(type.value(), value.value());
        }
        
        patternEntries.push_back(PatternEntry(type.value(), condition, value));
        if(checkNextCharacter(')'))
            isProcessed = true;
        else
            expectCharacter(',');
    }
    expectCharacter(-1); // EOT

    return patternEntries;
}