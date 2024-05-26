#pragma once
#include <filesystem>
#include <string>

auto checkIfValidExtension(std::filesystem::path const& path)->bool;
auto encodeImage(std::string const& path,std::string &message)->void;
auto decodeImage(std::string const& path)->void;