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
#include "PatternWrapper.h"

class Server {
private:
    std::string mq_name;
    mqd_t server_mqdes;
    std::map<int, int> clients;
    std::list<LindaTuple> tuples;
    std::list<PatternWrapper> patterns;

public:
    Server();
    ~Server();
    void Start();
    void Receive();
    void Send(int pid, int id, std::string data);

private:
    void InitServerMessageQueue();
    void MakeResponse(int pid, int id, LindaOperation, timespec tm, std::string);
    void HandleRead(PatternWrapper&);
    void HandleOutput(std::string &);
    void HandleInput(PatternWrapper&);
};
