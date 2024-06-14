#include <string>
#include "../graphic/graphic.hpp"
#include "fmt/core.h"

auto canOpenValidExtension(std::string const& path)->bool{
    if (checkIfValidExtension(path)){
        if(checkIfCanOpen(path)){
            return true;
        } else{
            fmt::println("Can't open this file :(");
            return false;
        }
    }else{
        fmt::println("Wrong extension!");
        return false;
    }
}
auto infoFlag(std::string const& path)->void{
    if(canOpenValidExtension(path)){
        getInfo(path);
    }else
        return;
}
auto encryptFlag(std::string const& path,std::string &message)->void{
    if (canOpenValidExtension(path))
        encryptImage(path,message);
    else
        return;
}
auto decryptFlag(std::string const& path)->void{
    if (canOpenValidExtension(path))
        decryptImage(path);
    else
        return;
}
auto checkFlag(std::string const& path, std::string const& message)->void{
    if (canOpenValidExtension(path)){
        canEncryptMessage(path,message);
    }else
        return;
}
auto helpFlag()->void{
    fmt::println("Help:"
                 "\n\tSupported Formats: bmp, ppm, tga"
                 "\n\tFlags: "
                 "\n\t\t[-i/--info] [path]"
                 "\n\t\t[-e/--encrypt] [path] [message]"
                 "\n\t\t[-d/--decrypt] [path]"
                 "\n\t\t[-c/--check] [path] [message]"
                 "\n\t\t[-h/--help]"
                 );
}