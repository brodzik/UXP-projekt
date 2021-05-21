#pragma once

#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "utility.h"

class Server {
private:
    std::string mq_name;
    mqd_t server_mqdes;
    std::map<int, int> clients;

public:
    Server();
    ~Server();
    void Start();
    void Receive();
    void Send(int pid, std::string data);

private:
    void InitServerMessageQueue();
};
