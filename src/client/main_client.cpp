#include "client.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        Client(getpid()).Start(false);
    } else if (argc == 2 && std::string(argv[1]) == "-i") {
        Client(getpid()).Start(true);
    }
    return 0;
}
