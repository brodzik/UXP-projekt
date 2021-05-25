#include "utility.h"

std::optional<LindaCommand> GetLindaCommand(std::string raw) {
    LindaCommand cmd = {};

    if (raw.length() >= 6 && raw.substr(0, 6) == "output") {
        cmd.op = OUTPUT;

        int first = raw.find("(");
        int last = raw.rfind(")");
        cmd.data = raw.substr(first + 1, last - first - 1);

        cmd.timeout = 0;
    } else if (raw.length() >= 5 && raw.substr(0, 5) == "input") {
        cmd.op = INPUT;

        int first1 = raw.find("(");
        int last1 = raw.rfind(",");
        cmd.data = raw.substr(first1 + 1, last1 - first1 - 1);

        int first2 = raw.rfind(",");
        int last2 = raw.rfind(")");
        cmd.timeout = std::stoi(raw.substr(first2 + 1, last2 - first2 - 1));
    } else if (raw.length() >= 4 && raw.substr(0, 4) == "read") {
        cmd.op = READ;

        int first1 = raw.find("(");
        int last1 = raw.rfind(",");
        cmd.data = raw.substr(first1 + 1, last1 - first1 - 1);

        int first2 = raw.rfind(",");
        int last2 = raw.rfind(")");
        cmd.timeout = std::stoi(raw.substr(first2 + 1, last2 - first2 - 1));
    } else {
        return std::nullopt;
    }

    return cmd;
}
