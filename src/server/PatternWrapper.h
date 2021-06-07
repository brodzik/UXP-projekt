#pragma once

#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <string>

#include "common/LindaPattern.h"
#include "common/LindaTuple.h"
#include "utility.h"

class PatternWrapper {
public:
    PatternWrapper(LindaPattern pattern, int pid, int id, timespec tm, bool ifDelete)
        : pattern(pattern)
        , pid(pid)
        , id(id)
        , tm(tm)
        , ifDelete(ifDelete) {
    }

    LindaPattern pattern;
    int pid;
    int id;
    timespec tm;
    bool ifDelete;

    bool HasTimedout() {
        if (tm.tv_sec == 0 && tm.tv_nsec == 0) {
            return false;
        }

        timespec curr;
        clock_gettime(CLOCK_REALTIME, &curr);
        return tm < curr;
    }
};
