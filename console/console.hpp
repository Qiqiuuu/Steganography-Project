#pragma once
auto infoFlag(std::string const& path)->void;
auto encryptFlag(std::string const& path,std::string &message)->void;
auto decryptFlag(std::string const& path)->void;
auto checkFlag(std::string const& path, std::string const& message)->void;
auto helpFlag()->void;