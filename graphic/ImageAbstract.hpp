#include <string>
#pragma once
struct ImageAbstract{
    virtual auto encryptMessage(std::string &message) -> void{};
    virtual auto decryptMessage() -> void{};
    virtual auto info() -> void{};
    virtual auto canEncrypt(std::string const &message) -> bool{};
};