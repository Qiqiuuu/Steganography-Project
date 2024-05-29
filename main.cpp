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
//wpisywanie wiadomosci ze spacja
//problem z flagami i ilosciami argumentów
//dodaj automatyczne tworzenie obiektów bmp i ppm
//zapytaj sie o pragme czy tak moze byc;
//zapytaj sie o format bmp 1,2,4,8,16,32
//zapytaj sie o timestamp

//fsystem::last_write_time

//dodaj polimorfizm
//zmien redundancje

    fmt::println("Welcome to Stegnography, write -h to see Help :)");
    while(true){
        auto in = std::string();
        std::getline(std::cin, in);
        auto stream = std::istringstream(in);
        auto input = std::vector<std::string>();
        auto next = std::string();
        while (stream >> next) {
            input.push_back(next);
        }
        if(input[0]=="exit"){
            break;
        }
        auto flag = input[0];
        if (flag == "-i" || flag == "-info") {
            infoFlag(input[1]);
        } else if (flag == "-e" || flag == "-encrypt") {
            encryptFlag(input[1],input[2]);
        } else if (flag == "-d" || flag == "-decrypt") {
            decryptFlag(input[1]);
        } else if (flag == "-c" || flag == "-check") {
            checkFlag(input[1],input[2]);
        } else if (flag == "-h" || flag == "-help") {
            helpFlag();
        } else {
            helpFlag();
        }
    }
    fmt::println("Good bye ;)");
}
