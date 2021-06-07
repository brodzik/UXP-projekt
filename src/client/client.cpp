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

    if (client_mqdes == -1) {
        std::cerr << "Error: failed to open client message queue." << std::endl;
        exit(-1);
    }
}

void Client::Start(bool interactive) {
    while (true) {
        if (interactive) {
            std::string s;
            std::getline(std::cin, s);

            if (s == "exit") {
                return;
            }

            Send(s);
        } else {
            Send("output((1, \"abc\", 3.1415, \"d\"))");
            sleep(1);
            Send("read((int: 1, string: *, float: *, string: *), 10)");
            sleep(1);
            Send("input((int: 1, string: *, float: *, string: *), 10)");
            sleep(1);
        }
    }
}

void Client::Send(std::string raw) {
    std::optional<LindaCommand> cmd;

    try {
        cmd = GetLindaCommand(raw);
        if (cmd.value().op == LindaOperation::OUTPUT) {
            LindaTuple(cmd->data);
        } else {
            LindaPattern(cmd->data);
        }
    } catch (...) {
        std::cerr << "Error: invalid Linda command." << std::endl;
        return;
    }

    struct timespec tm;
    size_t size = sizeof(pid) + sizeof(id) + sizeof(char) + sizeof(tm) + cmd->data.size();
    char *msg = (char *)malloc(size);
    memset(msg, 0, size);

    memcpy(msg, pid.bytes, sizeof(pid));
    memcpy(msg + sizeof(pid), id.bytes, sizeof(id));
    msg[sizeof(pid) + sizeof(id)] = cmd->op;

    if (cmd->timeout > 0) {
        clock_gettime(CLOCK_REALTIME, &tm);
        tm.tv_sec += cmd->timeout;
    } else {
        memset(&tm, 0, sizeof(tm));
    }

    memcpy(msg + sizeof(pid) + sizeof(id) + sizeof(char), &tm, sizeof(tm));

    memcpy(msg + sizeof(pid) + sizeof(id) + sizeof(char) + sizeof(tm), cmd->data.c_str(), cmd->data.size());

    if (mq_send(server_mqdes, msg, size, 0) < 0) {
        std::cerr << "Error: failed to send message." << std::endl;
    } else {
        std::cout << "Sent." << std::endl;
    }

    ++id.integer;

    delete msg;

    if (cmd->op != LindaOperation::OUTPUT) {
        Receive(cmd->timeout, tm);
    }
}

void Client::Receive(int timeout, timespec tm) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    ssize_t res;

    if (timeout <= 0) {
        res = mq_receive(client_mqdes, buffer, sizeof(buffer), NULL);
    } else {
        res = mq_timedreceive(client_mqdes, buffer, sizeof(buffer), NULL, &tm);
    }

    if (res < 0) {
        if (errno == ETIMEDOUT) {
            std::cout << "Timeout" << std::endl;
        } else {
            std::cerr << "Failed to receive message." << std::endl;
        }

        return;
    }

    IntBytes i;
    memcpy(i.bytes, buffer, 4);
    std::string data = std::string(buffer + 4);

    std::cout << "Got: " << i.integer << " " << data << std::endl;
}
