#include "AuxiliaryFunctions.h"
#include "utility.h"

TEST(parsingLindaCommands, output) {
    auto x = GetLindaCommand("output((1, \"abc\", 3.1415, \"d\"))");
    GTEST_ASSERT_EQ(x->op, OUTPUT);
    GTEST_ASSERT_EQ(x->data, "(1, \"abc\", 3.1415, \"d\")");
    GTEST_ASSERT_EQ(x->timeout, 0);
}

TEST(parsingLindaCommands, input) {
    auto x = GetLindaCommand("input((int: 1), 10)");
    GTEST_ASSERT_EQ(x->op, INPUT);
    GTEST_ASSERT_EQ(x->data, "(int: 1)");
    GTEST_ASSERT_EQ(x->timeout, 10);
}

TEST(parsingLindaCommands, read) {
    auto x = GetLindaCommand("read((int: 1), 10)");
    GTEST_ASSERT_EQ(x->op, READ);
    GTEST_ASSERT_EQ(x->data, "(int: 1)");
    GTEST_ASSERT_EQ(x->timeout, 10);
}
