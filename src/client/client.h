#pragma once

#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "common/LindaPattern.h"
#include "common/LindaTuple.h"
#include "utility.h"

class Client {
private:
    IntBytes pid;
    IntBytes id;

    std::string mq_name;

    mqd_t server_mqdes;
    mqd_t client_mqdes;

public:
    Client(int pid);
    ~Client();

    void Start(bool interactive);
    void Send(std::string raw, LindaOperation *op);
    void Receive();

private:
    void InitServerMessageQueue();
    void InitClientMessageQueue();
};
