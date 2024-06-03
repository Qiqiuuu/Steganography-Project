#include <filesystem>
#include "fstream"
#include "graphic.hpp"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "vector"
#include "bitset"
#include "ImageAbstract.hpp"

#pragma once

struct PPM : ImageAbstract{

    std::basic_fstream<char> inimage;
    std::string path;
    std::string format;
    int width;
    int height;
    int maxColorValue;
    size_t size;
    std::string bitsNeeded;
    int bits;
    auto setHeader(int setPixelInf)->void{
        std::string insert = "#" + std::to_string(setPixelInf) + " ";
        auto restFile = std::stringstream();
        inimage.seekg(bitsNeeded.size(), std::ios::beg);
        restFile << inimage.rdbuf();
        inimage.seekp(0);
        inimage << insert << restFile.str();
        inimage.seekg(0);
        inimage >> bitsNeeded >> format >> width >> height >> maxColorValue;
        bits = std::stoi(bitsNeeded.substr(1));
    }
    PPM(std::string const& path){
        this -> path = path;
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!inimage) {
            fmt::println("Can't open this file!");
        }
        auto buffer = std::string();
        inimage >> buffer;
        if(buffer[0]!='#'){
            bitsNeeded = "#0 ";
            auto restFile = std::stringstream();
            restFile << inimage.rdbuf();
            inimage.seekp(0);
            inimage << bitsNeeded << buffer << restFile.str();
            inimage.seekg(bitsNeeded.size(), std::ios::beg);
        }else
            bitsNeeded = buffer;
        inimage >> format >> width >> height >> maxColorValue;
        size = [this]()->size_t{
            size_t headerSize =0;
            headerSize += format.size() + 1;
            headerSize += std::to_string(width).size() + 1;
            headerSize += std::to_string(height).size() + 1;
            headerSize += std::to_string(maxColorValue).size() + 1;
            auto imgSize = width*height*3;
            return headerSize+imgSize+bitsNeeded.size();
        }();
        bits = std::stoi(bitsNeeded.substr(1));
    }

    auto canEncrypt(std::string const &message) -> bool override{
        return message.size() * 8 <= pow(2,16)|| message.size() * 8 <=width*height;
    }
    auto info()->void override{
        auto timeRaw = std::filesystem::last_write_time(std::filesystem::path(path));
        auto clock = std::chrono::clock_cast<std::chrono::system_clock>(timeRaw);
        auto timeT = std::chrono::system_clock::to_time_t(clock);
        fmt::println("Width: {}\nHeight: {}\nSize: {}kB\nLast modification time: {}",width,height,size/1024,std::ctime(&timeT));
    }
    auto encryptMessage(std::string &message) -> void override{
        auto pixelMessage = (message.size() * 8);
        setHeader(pixelMessage);
        auto charVec = std::vector<std::bitset<8>>();
        for (char c: message) {
            charVec.push_back(std::bitset<8>(c));
        }
        for(int i=0;i<pixelMessage;i++){
            auto bit = char();
            inimage.read((char *) (&bit), 1);
            bit &= ~1;
            if (charVec[i/8][7]) bit |= 1;
            charVec[i/8]<<=1;
            inimage.seekp(-1, std::ios::cur);
            inimage.write((char *) (&bit), 1);
        }
        fmt::println("Encrypted Correctly: {}",message);
    }
    auto decryptMessage() -> void override{
        auto message = std::string();
        auto charBit = std::string();
        for (int i = 1; i <= bits; i++) {
            auto bit = char();
            inimage.read((char *) (&bit), 1);
            charBit += std::to_string(std::bitset<8>(bit)[0]);
            if (i%8==0) {
                message += (char)std::bitset<8>(charBit).to_ulong();
                charBit = "";
            }
        };
        fmt::println("Decrypted Correctly: {}", message);
    }
};
