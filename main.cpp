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


auto main() -> int{
//to do!
//zerknij no otwieranie pliku
//przy ppm ogarnij komentarze wewnatrz

    fmt::println("Welcome to Stegnography, write -h to see Help :)");
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
                input[2] += next;
                if(stream) input[2] += " ";
            }
        }
        if(input[0]=="exit"){
            break;
        }
        auto flag = input[0];
        if (input.size()==2 && (flag == "-i" || flag == "-info")) {
            infoFlag(input[1]);
        } else if (input.size()==3 && (flag == "-e" || flag == "-encrypt")) {
            encryptFlag(input[1],input[2]);
        } else if (input.size()==2 && (flag == "-d" || flag == "-decrypt")) {
            decryptFlag(input[1]);
        } else if (input.size()==3 && (flag == "-c" || flag == "-check")) {
            checkFlag(input[1],input[2]);
        } else if ((input.size()==1 && (flag == "-h" || flag == "-help")) || (flag[0]!='-')) {
            helpFlag();
        } else {
            fmt::println("Invalid flag or amount of arguments. Check -h for help");
        }
    }
    fmt::println("Good bye ;)");
}
