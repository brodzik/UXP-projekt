#include "AuxiliaryFunctions.h"

TEST(patternTupleComparisons, basicInt) {
    LindaTuple lindaTuple(R"( (1) )");
    LindaPattern lindaPattern(R"( (int: 1) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple));
}

TEST(patternTupleComparisons, basicFloat) {
    LindaTuple lindaTuple(R"( (0.99) )");
    LindaPattern lindaPattern(R"( (float: < 1.00) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple));
}

TEST(patternTupleComparisons, basicString) {
    LindaTuple lindaTuple(R"( ("abc") )");
    LindaPattern lindaPattern(R"( (string: "abc") )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple));
}

TEST(patternTupleComparisons, conditionAny) {
    LindaTuple lindaTuple0(R"( (0) )");
    LindaTuple lindaTuple1(R"( (1) )");
    LindaTuple lindaTuple2(R"( (2) )");
    LindaPattern lindaPattern(R"( (int: *) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, conditionEqual) {
    LindaTuple lindaTuple0(R"( (0) )");
    LindaTuple lindaTuple1(R"( (1) )");
    LindaTuple lindaTuple2(R"( (2) )");
    LindaPattern lindaPattern(R"( (int: 1) )");
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, conditionLessEqual) {
    LindaTuple lindaTuple0(R"( (0) )");
    LindaTuple lindaTuple1(R"( (1) )");
    LindaTuple lindaTuple2(R"( (2) )");
    LindaPattern lindaPattern(R"( (int: <= 1) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, conditionLessThan) {
    LindaTuple lindaTuple0(R"( (0) )");
    LindaTuple lindaTuple1(R"( (1) )");
    LindaTuple lindaTuple2(R"( (2) )");
    LindaPattern lindaPattern(R"( (int: < 1) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, conditionMoreEqual) {
    LindaTuple lindaTuple0(R"( (0) )");
    LindaTuple lindaTuple1(R"( (1) )");
    LindaTuple lindaTuple2(R"( (2) )");
    LindaPattern lindaPattern(R"( (int: >= 1) )");
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, conditionMoreThan) {
    LindaTuple lindaTuple0(R"( (0) )");
    LindaTuple lindaTuple1(R"( (1) )");
    LindaTuple lindaTuple2(R"( (2) )");
    LindaPattern lindaPattern(R"( (int: > 1) )");
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, stringComparison) {
    LindaTuple lindaTuple0(R"( ("abb") )");
    LindaTuple lindaTuple1(R"( ("abc") )");
    LindaTuple lindaTuple2(R"( ("abd") )");
    LindaPattern lindaPattern(R"( (string: <= "abc") )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, stringComparisonUnequalLength1) {
    LindaTuple lindaTuple0(R"( ("ab") )");
    LindaTuple lindaTuple1(R"( ("a") )");
    LindaTuple lindaTuple2(R"( ("") )");
    LindaPattern lindaPattern(R"( (string: <= "abc") )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, stringComparisonUnequalLength2) {
    LindaTuple lindaTuple0(R"( ("ab") )");
    LindaTuple lindaTuple1(R"( ("a") )");
    LindaTuple lindaTuple2(R"( ("") )");
    LindaPattern lindaPattern(R"( (string: > "abc") )");
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, complex1) {
    LindaTuple lindaTuple0(R"( (1, 2, 3) )");
    LindaTuple lindaTuple1(R"( (1, 2) )");
    LindaTuple lindaTuple2(R"( (1, 2, 3, 4) )");
    LindaPattern lindaPattern(R"( (int: 1, int: 2, int: 3) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, complex2) {
    LindaTuple lindaTuple0(R"( (1, 2.01, "aazzzzzzz") )");
    LindaTuple lindaTuple1(R"( (1, 2.0, "aaa") )");
    LindaPattern lindaPattern(R"( (int: 1, float: > 2.0, string: < "abc") )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple1));
}

TEST(patternTupleComparisons, any) {
    LindaTuple lindaTuple0(R"( (1) )");
    LindaTuple lindaTuple1(R"( (101293) )");
    LindaTuple lindaTuple2(R"( (1.03) )");
    LindaTuple lindaTuple3(R"( ("abc") )");
    LindaPattern lindaPattern(R"( (int: *) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple3));
}

TEST(patternTupleComparisons, complexWithAny1) {
    LindaTuple lindaTuple0(R"( (1, 2.0, "c") )");
    LindaTuple lindaTuple1(R"( (1, "c") )");
    LindaTuple lindaTuple2(R"( (1, 2.0, 3.0) )");
    LindaPattern lindaPattern(R"( (int: 1, float: *, string: "c") )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
}

TEST(patternTupleComparisons, complexWithAny2) {
    LindaTuple lindaTuple0(R"( (1, 2, 3.0) )");
    LindaTuple lindaTuple1(R"( (1, 2) )");
    LindaTuple lindaTuple2(R"( (1, 2, 3, 4) )");
    LindaTuple lindaTuple3(R"( (1) )");
    LindaPattern lindaPattern(R"( (int: 1, int: *, float: *) )");
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple0));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple1));
    ASSERT_FALSE(lindaPattern.isMatching(lindaTuple2));
    ASSERT_TRUE(lindaPattern.isMatching(lindaTuple3));
}