#include "server.h"

#include <algorithm>

Server::Server() {
    mq_name = "/uxp_server_queue";

    InitServerMessageQueue();
}

Server::~Server() {
    mq_close(server_mqdes);
    mq_unlink(mq_name.c_str());
}

void Server::InitServerMessageQueue() {
    mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4096;

    mq_unlink(mq_name.c_str());

    server_mqdes = mq_open(mq_name.c_str(), O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG, &attr);

    if (server_mqdes == -1) {
        std::cerr << "Error: failed to open server message queue." << std::endl;
        exit(-1);
    }
}

void Server::Start() {
    while (true) {
        Receive();
    }
}

void Server::Receive() {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    ssize_t res = mq_receive(server_mqdes, buffer, sizeof(buffer), NULL);

    if (res == -1) {
        std::cerr << "Failed to receive message." << std::endl;
        return;
    }

    IntBytes pid = {};
    IntBytes id = {};

    memcpy(pid.bytes, buffer, 4);
    memcpy(id.bytes, buffer + 4, 4);

    LindaOperation op = (LindaOperation)buffer[8];

    if (clients.find(pid.integer) == clients.end()) {
        std::string path = "/uxp_client_queue_" + std::to_string(pid.integer);
        int mqdes = mq_open(path.c_str(), O_WRONLY);

        if (mqdes == -1) {
            std::cerr << "Error: failed to open message queue to client " << std::to_string(pid.integer) << std::endl;
        } else {
            clients[pid.integer] = mqdes;
        }
    }

    std::string data(buffer + 9);

    std::cout << "Got: " << pid.integer << " " << id.integer << " " << op << " " << data << std::endl;

    MakeResponse(pid.integer, op, data);
    //    Send(pid.integer, "OK");
}

void Server::HandleOutput(std::string &data) {
    LindaTuple tuple(data);

    auto it = find_if(patterns.begin(), patterns.end(), [&tuple](auto &patternWrapper) {
        return patternWrapper.pattern.isMatching(tuple);
    });

    if (it == patterns.end() || !(it->ifDelete)) {
        tuples.push_back(tuple);
    }

    if (it != patterns.end()) {
        Send(it->pid, tuple.toString());
        patterns.erase(it);
    }
}

void Server::HandleInput(int pid, std::string &data) {
    std::cout << data << std::endl;
    LindaPattern pattern(data);

    auto it = find_if(tuples.begin(), tuples.end(), [&pattern](auto &tuple) {
        return pattern.isMatching(tuple);
    });

    if (it != tuples.end()) {
        Send(pid, it->toString());
        tuples.erase(it);
    } else {
        PatternWrapper patternWrapper(pattern, pid, true);
        patterns.push_back(patternWrapper);
    }
}

void Server::HandleRead(int pid, std::string &data) {
    LindaPattern pattern(data);

    auto it = find_if(tuples.begin(), tuples.end(), [&pattern](auto &tuple) {
        return pattern.isMatching(tuple);
    });

    if (it != tuples.end()) {
        Send(pid, it->toString());
    } else {
        PatternWrapper patternWrapper(pattern, pid, false);
        patterns.push_back(patternWrapper);
    }
}

void Server::MakeResponse(int pid, LindaOperation op, std::string data) {
    switch (op) {
        case LindaOperation::OUTPUT:
            HandleOutput(data);
            break;

        case LindaOperation::READ:
            HandleRead(pid, data);
            break;

        case LindaOperation::INPUT:
            HandleInput(pid, data);
            break;
    }
}

void Server::Send(int pid, std::string data) {
    if (mq_send(clients[pid], data.c_str(), data.size(), 0) == -1) {
        std::cerr << "Error: failed to send message." << std::endl;
    }
}
