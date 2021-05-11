#include "client.h"

Client::Client() {
    mqdes = mq_open("/uxp_server_queue", O_WRONLY);

    if (mqdes == -1) {
        throw std::runtime_error("Failed to open message queue.");
    }
}

Client::~Client() {
    if (mq_close(mqdes) == -1) {
        //throw std::runtime_error("Failed to close message queue.");
    }
}

void Client::Run() {
    const char *msg = "test";

    for (int i = 0; i < 5; ++i) {
        mq_send(mqdes, msg, strlen(msg), 0);
    }
}
