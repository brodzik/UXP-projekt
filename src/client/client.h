#pragma once

#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <sys/stat.h>

#include <iostream>
#include <stdexcept>

class Client {
private:
    mqd_t mqdes;

public:
    Client();
    ~Client();
    void Run();
};
