#include <filesystem>
#include "fstream"
#include "graphic.hpp"
#include "fmt/core.h"
#include <string>
#include "BMP.hpp"




auto encodeImage(std::string const& path,std::string &message)->void{
    auto img = BMP(path);
    if(!img.checkIfCanEncode(message)) {
        fmt::println("Too long message/not enough pixels");
        return;
    }
    fmt::println("Message was encoded successfully! :D");
}
auto decodeImage(std::string const& path)->void{
    auto img = BMP(path);
    img.decodeMessage();
}
auto checkIfCanOpen(std::string const& path)->bool{
    return std::fstream(path).is_open();
}
auto checkIfValidExtension(std::string const& path)->bool{
    auto extension = std::filesystem::path(path).extension().string();
    return extension == ".ppm" || extension == ".bmp";
}
auto getExtension(std::string const& path)->std::string{
    auto extension = std::filesystem::path(path).extension().string();
    return extension;
}