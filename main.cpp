#include "graphic/graphic.hpp"
#include "console/console.hpp"
#include <filesystem>
#include "fmt/core.h"
#include <string>
#include <iostream>
#include "bitset"


auto main() -> int{
    auto path = "s1.bmp";
    if(!checkIfValidExtension(path)){
        fmt::println("Wrong Extension!");
        return 0;
    }
    auto str1 = std::string("lelan,smyk,tomasl");
    encodeImage(path, str1);
    decodeImage(path);
}
