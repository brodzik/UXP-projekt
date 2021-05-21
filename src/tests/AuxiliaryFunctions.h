#pragma once

#include <gtest/gtest.h>
#include "../common/LindaTuple.h"

inline void assertValue(Value tupleElement, Value expectedValue) {
    GTEST_ASSERT_EQ(tupleElement, expectedValue);
}