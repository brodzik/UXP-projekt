#include <gtest/gtest.h>

#include "common/LindaPattern.h"
#include "common/LindaTuple.h"
#include "server/server.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

