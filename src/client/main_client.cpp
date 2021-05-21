#include "client.h"

int main() {
    Client(getpid()).Start();
    return 0;
}
