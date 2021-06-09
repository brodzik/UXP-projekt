#include <fstream>
#include <streambuf>

#include <gtest/gtest.h>

#include <sys/types.h>
#include <unistd.h>

std::string readFile(const std::string& filename)
{
    std::ifstream file{filename, std::ifstream::in};
    std::stringstream s;
    s << file.rdbuf();
    return s.str();
}

TEST(System, simpleMsg) {
  system("../systemTests/scripts/simpleMsg.sh");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-simpleMsg.txt");
  EXPECT_EQ(groundtruth, output);
}

TEST(System, runCommand) {
  system("../systemTests/scripts/runCommands.sh complicatedMsg.txt");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-complicatedMsg.txt");
  EXPECT_EQ(groundtruth, output);
}

TEST(System, TwoClients) {
  system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  system("../systemTests/scripts/runCommands.sh receiver-1.txt");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");
  EXPECT_EQ(groundtruth, output);
}

TEST(System, TwoClientsReverseOutput) {
  system("../systemTests/scripts/runCommandsNoOutput.sh sender-2.txt");
  system("../systemTests/scripts/runCommands.sh receiver-1.txt");
  std::string output = readFile("output.txt");
  std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");
  EXPECT_EQ(groundtruth, output);
}

TEST(System, TwoClientsConcurrently) {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh receiver-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");
    EXPECT_EQ(groundtruth, output);
    exit(0);
  }

  else {
    system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  }
}


TEST(System, WaitAndReceive) {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh receiver-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");
    EXPECT_EQ(groundtruth, output);
    exit(0);
  }

  else {
    sleep(1);
    system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  }
}

TEST(System, TwoClientsConcurrentlyMixed) {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh client-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClientsMixed.txt");
    EXPECT_EQ(groundtruth, output);
    exit(0);
  }

  else {
    system("../systemTests/scripts/runCommandsNoOutput.sh client-2.txt");
  }
}

/*
TEST(System, TwoReceiversTwoSenders) {
  if (fork() == 0) {
    system("../systemTests/scripts/runCommands.sh receiver-1.txt");
    std::string output = readFile("output.txt");
    std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");
    EXPECT_EQ(groundtruth, output);
    exit(0);

    if (fork() == 0) {
      system("../systemTests/scripts/runCommands.sh receiver-1.txt");
      std::string output = readFile("output.txt");
      std::string groundtruth = readFile("../systemTests/scripts/groundtruth-twoClients.txt");
      EXPECT_EQ(groundtruth, output);
      exit(0);
    }

    else {
      system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
      exit(0);
    }
  }

  else {
    system("../systemTests/scripts/runCommandsNoOutput.sh sender-1.txt");
  }

}
*/

/*

TEST(System, serverNotFound) {
  system("../test/scripts/example.sh");

  std::string output = readFile("../test/scripts/output-example.txt");
  std::string groundtruth = readFile("../test/scripts/groundtruth-example.txt");

  EXPECT_EQ(groundtruth, output);
}

*/
/*
TEST(System, simpleMessage) {
  Server().Start();
  system("../test/scripts/simpleMessage.sh");

  std::string output = readFile("../test/scripts/output-simpleMessage.txt");
  std::string groundtruth = readFile("../test/scripts/groundtruth-simpleMessage.txt");

  EXPECT_EQ(groundtruth, output);
}
*/

