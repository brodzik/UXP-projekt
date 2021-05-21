#include "client.h"

Client::Client(int p) {
    pid = {.integer = p};
    id = {.integer = 0};
    mq_name = "/uxp_client_queue_" + std::to_string(pid.integer);

    InitServerMessageQueue();
    InitClientMessageQueue();
}

Client::~Client() {
    mq_close(server_mqdes);
    mq_close(client_mqdes);
    mq_unlink(mq_name.c_str());
}

void Client::InitServerMessageQueue() {
    server_mqdes = mq_open("/uxp_server_queue", O_WRONLY);

    if (server_mqdes == -1) {
        std::cerr << "Error: failed to open server message queue." << std::endl;
        exit(-1);
    }
}

void Client::InitClientMessageQueue() {
    mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4096;

    mq_unlink(mq_name.c_str());
    client_mqdes = mq_open(mq_name.c_str(), O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG, &attr);

    if (server_mqdes == -1) {
        std::cerr << "Error: failed to open client message queue." << std::endl;
        exit(-1);
    }
}

void Client::Start() {
    while (true) {
        Send("output(1, \"abc\", 3.1415, \"d\")");
        Receive();
        sleep(1);
    }
}

void Client::Send(std::string data) {
    size_t size = sizeof(pid) + sizeof(id) + data.size();
    char *msg = (char *)malloc(size);
    memset(msg, 0, size);

    memcpy(msg, pid.bytes, 4);
    memcpy(msg + 4, id.bytes, 4);
    memcpy(msg + 8, data.c_str(), data.size());

    if (mq_send(server_mqdes, msg, size, 0) == -1) {
        std::cerr << "Error: failed to send message." << std::endl;
    }

    ++id.integer;

    delete msg;
}

void Client::Receive() {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    ssize_t res = mq_receive(client_mqdes, buffer, sizeof(buffer), NULL);

    if (res == -1) {
        std::cerr << "Failed to receive message." << std::endl;
        return;
    }

    std::cout << "Got: " << std::string(buffer) << std::endl;
}
