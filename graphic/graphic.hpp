#pragma once
#include <filesystem>
#include <string>

auto checkIfValidExtension(std::string const& path)->bool;
auto encryptImage(std::string const& path,std::string &message)->void;
auto decryptImage(std::string const& path)->void;
auto getExtension(std::string const& path)->std::string;
auto checkIfCanOpen(std::string const& path)->bool;
auto getInfo(std::string const& path)->void;
auto canEncryptMessage(std::string const& path,std::string const&message)->void;