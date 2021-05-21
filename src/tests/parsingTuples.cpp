#include "AuxiliaryFunctions.h"

TEST(parsingTuples, basicInt) {
    LindaTuple lindaTuple(R"( (1) )");
    assertValue(lindaTuple.values[0], 1);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);
}


TEST(parsingTuples, basicFloat) {
    LindaTuple lindaTuple(R"( (3.14) )");
    assertValue(lindaTuple.values[0], 3.14f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);
}


TEST(parsingTuples, basicString) {
    LindaTuple lindaTuple(R"( ("abc") )");
    assertValue(lindaTuple.values[0], R"(abc)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);
}


TEST(parsingTuples, stringEscaping1) {
    LindaTuple lindaTuple(R"( ("\"") )");
    assertValue(lindaTuple.values[0], R"(")");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);
}


TEST(parsingTuples, stringEscaping2) {
    LindaTuple lindaTuple(R"( ("\\") )");
    assertValue(lindaTuple.values[0], R"(\)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 1);
}

TEST(parsingTuples, multipleInts) {
    LindaTuple lindaTuple(R"( (1, 2, 3) )");
    assertValue(lindaTuple.values[0], 1);
    assertValue(lindaTuple.values[1], 2);
    assertValue(lindaTuple.values[2], 3);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);
}

TEST(parsingTuples, multipleFloats) {
    LindaTuple lindaTuple(R"( (1.01, 2.02, 3.03) )");
    assertValue(lindaTuple.values[0], 1.01f);
    assertValue(lindaTuple.values[1], 2.02f);
    assertValue(lindaTuple.values[2], 3.03f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);
}

TEST(parsingTuples, multipleStrings) {
    LindaTuple lindaTuple(R"( ("a", "b", "c") )");
    assertValue(lindaTuple.values[0], R"(a)");
    assertValue(lindaTuple.values[1], R"(b)");
    assertValue(lindaTuple.values[2], R"(c)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);
}

TEST(parsingTuples, weirdSpacing) {
    LindaTuple lindaTuple(R"( (1   ,        2,3) )");
    assertValue(lindaTuple.values[0], 1);
    assertValue(lindaTuple.values[1], 2);
    assertValue(lindaTuple.values[2], 3);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);
}


TEST(parsingTuples, differentTypes1) {
    LindaTuple lindaTuple(R"( (1, 2.02, "abc") )");
    assertValue(lindaTuple.values[0], 1);
    assertValue(lindaTuple.values[1], 2.02f);
    assertValue(lindaTuple.values[2], R"(abc)");
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 3);
}


TEST(parsingTuples, differentTypes2) {
    LindaTuple lindaTuple(R"( ("text", 2, 3, 0.99) )");
    assertValue(lindaTuple.values[0], R"(text)");
    assertValue(lindaTuple.values[1], 2);
    assertValue(lindaTuple.values[2], 3);
    assertValue(lindaTuple.values[3], 0.99f);
    GTEST_ASSERT_EQ(lindaTuple.values.size(), 4);
}
