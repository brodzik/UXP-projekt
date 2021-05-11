#pragma once

#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <sys/stat.h>

#include <iostream>
#include <stdexcept>
#include <string>

class Server {
private:
    mqd_t mqdes;

public:
    Server();
    ~Server();
    void Run();
};
