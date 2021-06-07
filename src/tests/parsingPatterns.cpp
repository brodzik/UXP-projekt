#include "AuxiliaryFunctions.h"

TEST(parsingPatterns, basicInt) {
    // forward
    LindaPattern lindaPattern(R"( (int: 0) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::EQUAL, 0);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:0))");
}

TEST(parsingPatterns, basicFloat) {
    // forward
    LindaPattern lindaPattern(R"( (float: >1.00) )");
    assertPatternEntry(lindaPattern.entries[0], Type::FLOAT, 
        Condition::MORE_THAN, 1.0f);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((float:>1.000000))");
}

TEST(parsingPatterns, basicString) {
    // forward
    LindaPattern lindaPattern(R"( (string: "abc") )");
    assertPatternEntry(lindaPattern.entries[0], Type::STRING, Condition::EQUAL, 
        R"(abc)");
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((string:"abc"))");
}

TEST(parsingPatterns, multipleInts) {
    // forward
    LindaPattern lindaPattern(R"( (int: 1, int: 2, int: 3) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::EQUAL, 1);
    assertPatternEntry(lindaPattern.entries[1], Type::INT, Condition::EQUAL, 2);
    assertPatternEntry(lindaPattern.entries[2], Type::INT, Condition::EQUAL, 3);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:1,int:2,int:3))");
}

TEST(parsingPatterns, multipleFloats) {
    // forward
    LindaPattern lindaPattern(R"( (float: > 1.0, float: > 2.0, float: > 3.0))");
    assertPatternEntry(lindaPattern.entries[0], Type::FLOAT, 
        Condition::MORE_THAN, 1.0f);
    assertPatternEntry(lindaPattern.entries[1], Type::FLOAT, 
        Condition::MORE_THAN, 2.0f);
    assertPatternEntry(lindaPattern.entries[2], Type::FLOAT, 
        Condition::MORE_THAN, 3.0f);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), 
        R"((float:>1.000000,float:>2.000000,float:>3.000000))");
}

TEST(parsingPatterns, multipleStrings) {
    // forward
    LindaPattern lindaPattern(R"( (string: "a", string: "b", string: "c") )");
    assertPatternEntry(lindaPattern.entries[0], Type::STRING, Condition::EQUAL, 
        R"(a)");
    assertPatternEntry(lindaPattern.entries[1], Type::STRING, Condition::EQUAL, 
        R"(b)");
    assertPatternEntry(lindaPattern.entries[2], Type::STRING, Condition::EQUAL, 
        R"(c)");
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), 
        R"((string:"a",string:"b",string:"c"))");
}

TEST(parsingPatterns, conditionAny) {
    // forward
    LindaPattern lindaPattern(R"( (int: *) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::ANY, 
        std::nullopt);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:*))");
}

TEST(parsingPatterns, conditionMoreThan) {
    // forward
    LindaPattern lindaPattern(R"( (int: > 1) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::MORE_THAN, 
        1);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:>1))");
}

TEST(parsingPatterns, conditionMoreEqual) {
    // forward
    LindaPattern lindaPattern(R"( (int: >= 1) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, 
        Condition::MORE_EQUAL, 1);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:>=1))");
}

TEST(parsingPatterns, conditionLessThan) {
    // forward
    LindaPattern lindaPattern(R"( (int: < 1) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::LESS_THAN, 
        1);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:<1))");
}

TEST(parsingPatterns, conditionLessEqual) {
    // forward
    LindaPattern lindaPattern(R"( (int: <= 1) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, 
        Condition::LESS_EQUAL, 1);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:<=1))");
}

TEST(parsingPatterns, weirdSpacing) {
    // forward
    LindaPattern lindaPattern(R"((      int:1,int:    2,   
        int: 3) )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::EQUAL, 1);
    assertPatternEntry(lindaPattern.entries[1], Type::INT, Condition::EQUAL, 2);
    assertPatternEntry(lindaPattern.entries[2], Type::INT, Condition::EQUAL, 3);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), R"((int:1,int:2,int:3))");
}

TEST(parsingPatterns, differentTypes1) {
    // forward
    LindaPattern lindaPattern(R"( (int: 1, float: > 2.0, string: "abc") )");
    assertPatternEntry(lindaPattern.entries[0], Type::INT, Condition::EQUAL, 1);
    assertPatternEntry(lindaPattern.entries[1], Type::FLOAT, 
        Condition::MORE_THAN, 2.0f);
    assertPatternEntry(lindaPattern.entries[2], Type::STRING, Condition::EQUAL, 
        R"(abc)");
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), 
        R"((int:1,float:>2.000000,string:"abc"))");
}

TEST(parsingPatterns, differentTypes2) {
    // forward
    LindaPattern lindaPattern(R"( (string: "text", int: 2, int: 3, 
        float: <= 0.99) )");
    assertPatternEntry(lindaPattern.entries[0], Type::STRING, Condition::EQUAL, 
        R"(text)");
    assertPatternEntry(lindaPattern.entries[1], Type::INT, Condition::EQUAL, 2);
    assertPatternEntry(lindaPattern.entries[2], Type::INT, Condition::EQUAL, 3);
    assertPatternEntry(lindaPattern.entries[3], Type::FLOAT, 
        Condition::LESS_EQUAL, 0.99f);
    GTEST_ASSERT_EQ(lindaPattern.entries.size(), 4);

    // reverse
    GTEST_ASSERT_EQ(lindaPattern.toString(), 
        R"((string:"text",int:2,int:3,float:<=0.990000))");
}

TEST(parsingPatterns, floatEqualsFail) {
    // forward
    try {
        LindaPattern lindaPattern(R"( (float: 1.00) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 9 -> Condition EQUAL for type FLOAT is illegal");
    }
}

TEST(parsingPatterns, typeMismatchFail) {
    // forward
    try {
        LindaPattern lindaPattern(R"( (int: 1.23) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 11 -> Expected value to be of type INT");
    }
}

TEST(parsingPatterns, incorrectTypeFail) {
    // forward
    try {
        LindaPattern lindaPattern(R"( (double: 1.23) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 8 -> Expected type");
    }
}

TEST(parsingPatterns, missingColonFail) {
    // forward
    try {
        LindaPattern lindaPattern(R"( (int; 1) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 5 -> Expected \":\", but got \";\"");
    }
}

TEST(parsingPatterns, incorrectConditionSpacingFail) {
    // forward
    try {
        LindaPattern lindaPattern(R"( (int: > = 1) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 9 -> Expected number or string");
    }
}
