#include "graphic/graphic.hpp"
#include "console/console.hpp"
#include <filesystem>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <string>
#include <vector>

auto getArgumentsToVector(int argc, char* argv[]) -> std::vector<std::string> {
    auto args = std::vector<std::string>();
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return args;
}
auto main(int argc, char* argv[]) -> int {
    if(argc == 1)helpFlag();
    if (argc > 1) {
        auto input = getArgumentsToVector(argc, argv);
        auto flag = input[0];
        if (input.size()==2 && (flag == "-i" || flag == "--info")) {
            infoFlag(input[1]);
        } else if (input.size()==3 && (flag == "-e" || flag == "--encrypt")) {
            encryptFlag(input[1],input[2]);
        } else if (input.size()==2 && (flag == "-d" || flag == "--decrypt")) {
            decryptFlag(input[1]);
        } else if (input.size()==3 && (flag == "-c" || flag == "--check")) {
            checkFlag(input[1],input[2]);
        } else if ((input.size()==1 && (flag == "-h" || flag == "--help")) || (flag[0]!='-' || flag[0]!='-' && flag[1]!='-')) {
            helpFlag();
        } else {
            fmt::println("Invalid flag or amount of arguments. Check -h for help");
            return 1;
        }
    }
    return 0;
}
