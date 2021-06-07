#pragma once

#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <string>

#include "common/LindaPattern.h"
#include "common/LindaTuple.h"
#include "utility.h"

class Server {
private:
    struct PatternWrapper {
        PatternWrapper(LindaPattern pattern, int pid, int id, bool ifDelete)
            : pattern(pattern)
            , pid(pid)
            , id(id)
            , ifDelete(ifDelete) {
        }
        LindaPattern pattern;
        int pid;
        int id;
        bool ifDelete;
    };

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
    void MakeResponse(int pid, int id, LindaOperation, std::string);
    void HandleRead(int pid, int id, std::string &);
    void HandleOutput(std::string &);
    void HandleInput(int pid, int id, std::string &);
};
