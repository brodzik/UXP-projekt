#include "server.h"

Server::Server() {
    // Set message queue attributes
    mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4096;

    // Destroy existing message queue
    if (mq_unlink("/uxp_server_queue") == -1) {
    }

    // Create new message queue
    mqdes = mq_open("/uxp_server_queue", O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG, &attr);

    if (mqdes == -1) {
        throw std::runtime_error("Failed to open message queue.");
    }
}

Server::~Server() {
    if (mq_close(mqdes) == -1) {
    }

    if (mq_unlink("/uxp_server_queue") == -1) {
    }
}

void Server::Run() {
    while (true) {
        char buffer[4096];
        ssize_t res = mq_receive(mqdes, buffer, sizeof(buffer), NULL);

        if (res == -1) {
            throw std::runtime_error("Failed to receive message.");
        }

        std::cout << "Got: " << std::string(buffer) << std::endl;
    }
}
