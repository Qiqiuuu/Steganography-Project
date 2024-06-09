#include "graphic/graphic.hpp"
#include "console/console.hpp"
#include <filesystem>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <string>
#include <iostream>
#include <vector>
#include "bitset"
#include "console/console.hpp"
#include "regex"
auto main() -> int{
//to do!
//zerknij no otwieranie pliku
//przy ppm ogarnij komentarze wewnatrz

    fmt::println("Welcome to Steganography, write -h to see Help :)");
    while(true){
        auto in = std::string();
        std::getline(std::cin, in);
        auto stream = std::istringstream(in);
        auto input = std::vector<std::string>();
        auto next = std::string();
        while (stream >> next) {
            if(input.size()!=3)
                input.push_back(next);
            else{
                if (!input[2].empty()) {
                    input[2] += " ";
                }
                input[2] += next;
            }
        }
        if(input.size()>2){
            auto pattern = std::regex("\"(.*)\"");
            auto match = std::smatch();
            if (std::regex_search(input[2], match, pattern))
                input[2] = match[0].str().substr(1, match[0].str().size() - 2);
            else{
                fmt::println("You didn't provide your message correcly");
                continue;
            }
        }
        if(input[0]=="exit"){
            break;
        }
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
        }
    }
    fmt::println("Good bye ;)");
}
