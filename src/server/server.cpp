#include "server.h"

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

    if (clients.find(pid.integer) == clients.end()) {
        std::string path = "/uxp_client_queue_" + std::to_string(pid.integer);
        int mqdes = mq_open(path.c_str(), O_WRONLY);

        if (mqdes == -1) {
            std::cerr << "Error: failed to open message queue to client " << std::to_string(pid.integer) << std::endl;
        } else {
            clients[pid.integer] = mqdes;
        }
    }

    std::cout << "Got: " << pid.integer << " " << id.integer << " " << std::string(buffer + 8) << std::endl;

    Send(pid.integer, "OK");
}

void Server::Send(int pid, std::string data) {
    if (mq_send(clients[pid], data.c_str(), data.size(), 0) == -1) {
        std::cerr << "Error: failed to send message." << std::endl;
    }
}
