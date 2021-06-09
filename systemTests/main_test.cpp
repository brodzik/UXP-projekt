#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <cassert>
#include <sstream>
#include <iostream>

void test_simpleMsg();
void test_complicatedMsg();
void test_twoClientsConcurrentlyMixed();
void test_waitAndReceive();
void test_twoClients();
void test_twoClientsReverseOutput();
void test_twoClientsConcurrently();
void test_multipleClients();

std::string readFile(const std::string& filename)
{
    std::ifstream file{filename, std::ifstream::in};
    std::stringstream s;
    s << file.rdbuf();
    return s.str();
}

int main() {
  std::cout << "test simpleMsg" << std::endl;
  test_simpleMsg();

  std::cout << "test twoClients" << std::endl;
  test_twoClients();

  std::cout << "test twoClientsConcurrentlyMixed" << std::endl;
  test_twoClientsConcurrentlyMixed();
  usleep(100000);

  std::cout << "test twoClientsReverseOutput" << std::endl;
  test_twoClientsReverseOutput();
  usleep(100000);

  std::cout << "test twoClientsConcurrently" << std::endl;
  test_twoClientsConcurrently();
  usleep(100000);

  std::cout << "test multipleClients" << std::endl;
  test_multipleClients();
  usleep(100000);


  std::cout << "test complicatedMsg" << std::endl;
  test_complicatedMsg();
  usleep(10000);

  std::cout << "test waitAndReceive" << std::endl;
  test_waitAndReceive();

}

void test_simpleMsg() {
  std::system("../systemTests/scripts/simpleMsg.sh");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-simpleMsg.txt");


  std::cout << "OUTPUT:" << std::endl;
  std::cout << output << std::endl;

  assert(output == groundtruth);
}

// na prezentacje
void test_twoClientsConcurrentlyMixed() {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh client-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClientsMixed.txt");

    std::cout << "OUTPUT:" << std::endl;
    std::cout << output << std::endl;

    assert(output == groundtruth);
    exit(0);
  }

  else {
    system("../systemTests/scripts/runCommandsNoOutput.sh client-2.txt");
  }
}

void test_waitAndReceive() {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh receiver-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");

    std::cout << "OUTPUT:" << std::endl;
    std::cout << output << std::endl;

    assert(groundtruth == output);
    exit(0);
  }

  else {
    sleep(1);
    system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  }
}

void test_twoClients() {
  system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  system("../systemTests/scripts/runCommands.sh receiver-1.txt");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");

  std::cout << "OUTPUT:" << std::endl;
  std::cout << output << std::endl;

  assert(groundtruth == output);
}

void test_twoClientsReverseOutput() {
  system("../systemTests/scripts/runCommandsNoOutput.sh sender-2.txt");
  system("../systemTests/scripts/runCommands.sh receiver-1.txt");

  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");

  std::cout << "OUTPUT:" << std::endl;
  std::cout << output << std::endl;

  assert(groundtruth == output);
}

// na prezentacje
void test_twoClientsConcurrently() {
  if (fork() == 0) {
    usleep(10000);
    system("../systemTests/scripts/runCommands.sh receiver-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");

    std::cout << "OUTPUT:" << std::endl;
    std::cout << output << std::endl;

    assert(groundtruth == output);
    exit(0);
  }

  else {
    system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  }
}

// na prezentacje
void test_multipleClients() {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh receiver-multipleClients.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-multipleClients.txt");

    std::cout << "OUTPUT:" << std::endl;
    std::cout << output << std::endl;

    assert(groundtruth == output);
    exit(0);
  }
  else {
    for (int i = 0; i < 6; ++i) {
      if (fork() == 0) {
        system("../systemTests/scripts/runCommandsNoOutput.sh sender-2.txt");
        exit(0);
      }
    }
  }
}

void test_complicatedMsg() {
  system("../systemTests/scripts/runCommands.sh complicatedMsg.txt");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-complicatedMsg.txt");

  std::cout << "OUTPUT:" << std::endl;
  std::cout << output << std::endl;

  assert(groundtruth == output);
}
