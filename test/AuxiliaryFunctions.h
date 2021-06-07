#pragma once

#include <gtest/gtest.h>

#include "common/LindaPattern.h"
#include "common/LindaTuple.h"

inline void assertValue(Value tupleElement, Value expectedValue) {
    GTEST_ASSERT_EQ(tupleElement, expectedValue);
}

inline void assertPatternEntry(PatternEntry patternEntry, Type type,
                               Condition condition, std::optional<Value> value) {
    GTEST_ASSERT_EQ(patternEntry.type, type);
    GTEST_ASSERT_EQ(patternEntry.condition, condition);
    GTEST_ASSERT_EQ(patternEntry.value, value);
}
