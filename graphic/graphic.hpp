#pragma once
#include <filesystem>
#include <string>

auto checkIfValidExtension(std::string const& path)->bool;
auto encodeImage(std::string const& path,std::string &message)->void;
auto decodeImage(std::string const& path)->void;
auto getExtension(std::string const& path)->std::string;
auto checkIfCanOpen(std::string const& path)->bool;