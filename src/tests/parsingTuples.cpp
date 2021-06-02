#include "AuxiliaryFunctions.h"

TEST(parsingTuples, basicInt) {
    // forward
    LindaTuple lindaTuple(R"( (1) )");
    assertValue(lindaTuple.values[0], 1);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((1))");
}


TEST(parsingTuples, basicFloat) {
    // forward
    LindaTuple lindaTuple(R"( (3.14) )");
    assertValue(lindaTuple.values[0], 3.14f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((3.140000))");
}


TEST(parsingTuples, basicString) {
    // forward
    LindaTuple lindaTuple(R"( ("abc") )");
    assertValue(lindaTuple.values[0], R"(abc)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"(("abc"))");
}


TEST(parsingTuples, stringWithSpaces) {
    // forward
    LindaTuple lindaTuple(R"( ("abc def") )");
    assertValue(lindaTuple.values[0], R"(abc def)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"(("abc def"))");
}


TEST(parsingTuples, stringEscaping1) {
    // forward
    LindaTuple lindaTuple(R"( ("\"") )");
    assertValue(lindaTuple.values[0], R"(")");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"(("\""))");
}


TEST(parsingTuples, stringEscaping2) {
    // forward
    LindaTuple lindaTuple(R"( ("\\") )");
    assertValue(lindaTuple.values[0], R"(\)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"(("\\"))");
}

TEST(parsingTuples, multipleInts) {
    // forward
    LindaTuple lindaTuple(R"( (1, 2, 3) )");
    assertValue(lindaTuple.values[0], 1);
    assertValue(lindaTuple.values[1], 2);
    assertValue(lindaTuple.values[2], 3);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((1,2,3))");
}

TEST(parsingTuples, multipleFloats) {
    // forward
    LindaTuple lindaTuple(R"( (1.01, 2.02, 3.03) )");
    assertValue(lindaTuple.values[0], 1.01f);
    assertValue(lindaTuple.values[1], 2.02f);
    assertValue(lindaTuple.values[2], 3.03f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((1.010000,2.020000,3.030000))");
}

TEST(parsingTuples, multipleStrings) {
    // forward
    LindaTuple lindaTuple(R"( ("a", "b", "c") )");
    assertValue(lindaTuple.values[0], R"(a)");
    assertValue(lindaTuple.values[1], R"(b)");
    assertValue(lindaTuple.values[2], R"(c)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"(("a","b","c"))");
}

TEST(parsingTuples, weirdSpacing) {
    // forward
    LindaTuple lindaTuple(R"( (1   ,        2,3) )");
    assertValue(lindaTuple.values[0], 1);
    assertValue(lindaTuple.values[1], 2);
    assertValue(lindaTuple.values[2], 3);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((1,2,3))");
}


TEST(parsingTuples, differentTypes1) {
    // forward
    LindaTuple lindaTuple(R"( (1, 2.02, "abc") )");
    assertValue(lindaTuple.values[0], 1);
    assertValue(lindaTuple.values[1], 2.02f);
    assertValue(lindaTuple.values[2], R"(abc)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((1,2.020000,"abc"))");
}


TEST(parsingTuples, differentTypes2) {
    // forward
    LindaTuple lindaTuple(R"( ("text", 2, 3, 0.99) )");
    assertValue(lindaTuple.values[0], R"(text)");
    assertValue(lindaTuple.values[1], 2);
    assertValue(lindaTuple.values[2], 3);
    assertValue(lindaTuple.values[3], 0.99f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 4);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"(("text",2,3,0.990000))");
}


TEST(parsingTuples, floatThatIsAnInt) {
    // forward
    LindaTuple lindaTuple(R"( (1.) )");
    assertValue(lindaTuple.values[0], 1.0f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);

    // reverse
    GTEST_ASSERT_EQ(lindaTuple.toString(), R"((1.000000))");
}

TEST(parsingTuples, leadingZerosFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( (01) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 3 -> Leading zeros not allowed");
    }
}

TEST(parsingTuples, numberTooBigFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( (99999999999999) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 11 -> Number too big");
    }
}

TEST(parsingTuples, UnexpectedEndOfTextFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( ("abc) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 8 -> Unexpected end of text");
    }
}

TEST(parsingTuples, incorrectStartingCharacterFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( 1) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 1 -> Expected \"(\", but got \"1\"");
    }
}

TEST(parsingTuples, noNumberOrTextFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( (abc) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 2 -> Expected number or string");
    }
}

TEST(parsingTuples, missingCommaFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( (1 2) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 4 -> Expected \",\", but got \"2\"");
    }
}

TEST(parsingTuples, trailingCommaFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( (1,) )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 4 -> Expected number or string");
    }
}

TEST(parsingTuples, trailingCharacterFail) {
    // forward
    try {
        LindaTuple lindaTuple(R"( (1)a )");
    }
    catch(std::string exception) {
        GTEST_ASSERT_EQ(exception, 
            "Position: 4 -> Expected \"\xFF\", but got \"a\"");
    }
}
