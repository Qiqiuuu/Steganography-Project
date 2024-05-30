#include <filesystem>
#include "fstream"
#include "graphic.hpp"
#include "fmt/core.h"
#include <string>
#include "BMP.hpp"


auto returnImage(std::string const& path)-> ImageAbstract*{
    if(getExtension(path)==".bmp")
        return new BMP(path);
    return nullptr;
}
auto checkPTR(ImageAbstract* img) -> bool {
    if (!img) {
        fmt::println("Failed to load image :(");
        return false;
    }
    return true;
}

auto encryptImage(std::string const& path,std::string &message)->void{
    auto img = returnImage(path);
    if (!checkPTR(img)){
        delete img;
        return;
    }
    if(!(img ->canEncrypt(message))){
        fmt::println("Too long message/not enough pixels");
        delete img;
        return;
    }
    img->encryptMessage(message);
    delete img;
}
auto decryptImage(std::string const& path)->void{
    auto img = returnImage(path);
    if (!checkPTR(img)){
        delete img;
        return;
    }
    img->decryptMessage();
    delete img;
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
auto getInfo(std::string const& path)->void{
    auto img = returnImage(path);
    if (!checkPTR(img)){
        delete img;
        return;
    }
    img->info();
    delete img;
}
auto canEncryptMessage(std::string const& path,std::string const&message)->void{
    auto img = returnImage(path);
    if (!checkPTR(img)){
        delete img;
        return;
    }
    if(img ->canEncrypt(message))
        fmt::println("You can encrypt message in this image");
    else
        fmt::println("Too long message/not enough pixels");
    delete img;
}