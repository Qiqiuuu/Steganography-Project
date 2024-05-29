#include <string>
#include "../graphic/graphic.hpp"
#include "fmt/core.h"
#include "../graphic/BMP.hpp"


auto infoFlag(std::string const& path)->void{
    if (checkIfValidExtension(path)){
        if(checkIfCanOpen(path)){
            if(getExtension(path)==".bmp"){
                auto img = BMP(path);
                img.info();
            }else if(getExtension(path)==".ppm"){

            }
        } else
            fmt::println("Can't open this file :(");
    }else
        fmt::println("Wrong extension!");
}
auto encryptFlag(std::string const& path,std::string &message)->void{
    if (checkIfValidExtension(path)){
        if(checkIfCanOpen(path)){
                encodeImage(path,message);
        } else
            fmt::println("Can't open this file :(");
    }else
        fmt::println("Wrong extension!");
}
auto decryptFlag(std::string const& path)->void{
    if (checkIfValidExtension(path)){
        if(checkIfCanOpen(path)){
            decodeImage(path);
        } else
            fmt::println("Can't open this file :(");
    }else
        fmt::println("Wrong extension!");
}
auto checkFlag(std::string const& path, std::string const& message)->void{
    if (checkIfValidExtension(path)){
        if(checkIfCanOpen(path)){
            auto img = BMP(path);
            if(img.checkIfCanEncode(message))fmt::println("You can encrypt this message!");
            else fmt::println("Too long message/not enough pixels");
        } else
            fmt::println("Can't open this file :(");
    }else
        fmt::println("Wrong extension!");
}
auto helpFlag()->void{
    fmt::println("Help:"
                 "\n\tSupported Formats: bmp, ppm"
                 "\n\tFlags: "
                 "\n\t\t[-i/-info] [path]"
                 "\n\t\t[-e/-encrypt] [path] [message]"
                 "\n\t\t[-d/-decrypt] [path]"
                 "\n\t\t[-c/-check] [path] [message]"
                 "\n\t\t[-h/-help]"
                 "\nexit -> end program"
                 );
}