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

    memcpy(pid.bytes, buffer, sizeof(pid));
    memcpy(id.bytes, buffer + sizeof(pid), sizeof(id));

    LindaOperation op = (LindaOperation)buffer[sizeof(pid) + sizeof(id)];

    struct timespec tm;
    memcpy(&tm, buffer + sizeof(pid) + sizeof(id) + sizeof(char), sizeof(tm));

    if (clients.find(pid.integer) == clients.end()) {
        std::string path = "/uxp_client_queue_" + std::to_string(pid.integer);
        int mqdes = mq_open(path.c_str(), O_WRONLY);

        if (mqdes == -1) {
            std::cerr << "Error: failed to open message queue to client " << std::to_string(pid.integer) << std::endl;
        } else {
            clients[pid.integer] = mqdes;
        }
    }

    std::string data(buffer + sizeof(pid) + sizeof(id) + sizeof(char) + sizeof(tm));
    std::cout << "Got: " << pid.integer << " " << id.integer << " " << op << " " << data << std::endl;

    try {
        MakeResponse(pid.integer, id.integer, op, tm, data);
    } catch (...) {
        std::cerr << "error" << std::endl;
    }
}

void Server::HandleOutput(std::string &data) {
    LindaTuple tuple(data);

    auto it = std::find_if(patterns.begin(), patterns.end(), [&](auto &patternWrapper) {
        return patternWrapper.pattern.isMatching(tuple);
    });

    while (it != patterns.end()) {
        if (!(it->ifDelete)) {
            tuples.push_back(tuple);
            return;
        }

        if (HasTimedout(it->tm)) {
            std::cout << "Request " << it->pid << " " << it->id << " has already timed out. Tuple won't be sent." << std::endl;
            patterns.erase(it);
        } else {
            Send(it->pid, it->id, tuple.toString());
            patterns.erase(it);
            return;
        }

        it = std::find_if(patterns.begin(), patterns.end(), [&](auto &patternWrapper) {
            return patternWrapper.pattern.isMatching(tuple);
        });
    }

    if (it == patterns.end() || !(it->ifDelete)) {
        tuples.push_back(tuple);
        return;
    }
}

void Server::HandleInput(int pid, int id, timespec tm, std::string &data) {
    LindaPattern pattern(data);

    auto it = std::find_if(tuples.begin(), tuples.end(), [&](auto &tuple) {
        return pattern.isMatching(tuple);
    });

    if (it != tuples.end() && !HasTimedout(tm)) {
        Send(pid, id, it->toString());
        tuples.erase(it);
    } else {
        PatternWrapper patternWrapper(pattern, pid, id, tm, true);
        patterns.push_back(patternWrapper);
    }
}

void Server::HandleRead(int pid, int id, timespec tm, std::string &data) {
    LindaPattern pattern(data);

    auto it = std::find_if(tuples.begin(), tuples.end(), [&](auto &tuple) {
        return pattern.isMatching(tuple);
    });

    if (it != tuples.end() && !HasTimedout(tm)) {
        Send(pid, id, it->toString());
    } else {
        PatternWrapper patternWrapper(pattern, pid, id, tm, false);
        patterns.push_back(patternWrapper);
    }
}

void Server::HandleExit(int pid) {
    auto it = clients.find(pid);

    if (it != clients.end()) {
        std::string path = "/uxp_client_queue_" + std::to_string(pid);
        mq_close(it->second);
        mq_unlink(path.c_str());
        clients.erase(it);
        std::cout << "Connection to " << pid << " has been closed." << std::endl;
    }
}

void Server::MakeResponse(int pid, int id, LindaOperation op, timespec tm, std::string data) {
    switch (op) {
        case LindaOperation::OUTPUT:
            HandleOutput(data);
            break;

        case LindaOperation::READ:
            HandleRead(pid, id, tm, data);
            break;

        case LindaOperation::INPUT:
            HandleInput(pid, id, tm, data);
            break;

        case LindaOperation::EXIT:
            HandleExit(pid);
            break;
    }
}

void Server::Send(int pid, int id, std::string data) {
    IntBytes i = {.integer = id};

    size_t size = sizeof(i) + data.size();
    char *msg = (char *)malloc(size);
    memset(msg, 0, size);

    memcpy(msg, i.bytes, 4);
    memcpy(msg + 4, data.c_str(), data.size());

    if (mq_send(clients[pid], msg, size, 0) < 0) {
        std::cerr << "Error: failed to send message." << std::endl;
    } else {
        std::cout << "Response sent " << pid << " " << id << "." << std::endl;
    }

    delete msg;
}

bool Server::HasTimedout(timespec tm) {
    if (tm.tv_sec == 0 && tm.tv_nsec == 0) {
        return false;
    }

    timespec curr;
    clock_gettime(CLOCK_REALTIME, &curr);
    return tm < curr;
}
