#include <fstream>
#include <streambuf>
#include <gtest/gtest.h>
#include "server/server.h"

std::string readFile(const std::string& filename)
{
    std::ifstream file{filename, std::ifstream::in};
    std::stringstream s;
    s << file.rdbuf();
    return s.str();
}

TEST(System, example) {
//  Server server;
//  server.run();

  system("../test/scripts/example.sh");

  std::string output = readFile("../test/scripts/output-example.txt");
  std::string groundtruth = readFile("../test/scripts/groundtruth-example.txt");

  EXPECT_EQ(groundtruth, output);
}

/*
TEST(FrostfruitCharacterizationTest, Foo)
{
    Frostfruit app
    app.addItem(new NormalItem("+7 Yellow Vest", 10, 20));
    app.addItem(new CheeseItem("Cheese Brie", 10, 20));
    app.addItem(new ConcertTicketItem("Tickets to a concert", 10, 20));
    app.addItem(new LegolasItem("Legolas, Hand of Gollum", 10, 20));

    std::stringstream output;

    for (int day = 0; day < 21; ++day)
    {
        output << "Day is " << day << std::endl;
        app.updateQuality();
        app.printItems(output);
        if (day < 20) {
          output << std::endl;
        }
    }

    std::string groundtruth = readGroundtruth("../ExpectedTestOutput.txt");
    EXPECT_EQ(groundtruth, output.str());
}
*/
