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
#pragma pack(push, 1)
struct TARGAFileHeader {
    char idLength; // Length of the identifier
    char colorMapType; // Color map type
    char imageType; // Image type
    uint16_t colorMapOrigin; // Color map origin
    uint16_t colorMapLength; // Color map length
    char colorMapDepth; // Color map depth
    uint16_t xOrigin; // X-origin of the upper-left corner
    uint16_t yOrigin; // Y-origin of the upper-left corner
    uint16_t width; // Width of the image
    uint16_t height; // Height of the image
    char bitDepth; // Bits per pixel
    char imageDescriptor; // Image descriptor
};
#pragma pack(pop)


struct TARGA: ImageAbstract{
    TARGAFileHeader fileHeader;
    std::basic_fstream<char> inimage;
    std::string path;
    int size;

    TARGA(std::string const &path) {
        this -> path = path;
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!inimage) {
            fmt::println("Can't open this file!");
        }
        inimage.read((char*)&fileHeader, sizeof(fileHeader));
        size = sizeof(fileHeader)+fileHeader.width*fileHeader.height*fileHeader.bitDepth;
    }

    auto setHeader(int setPixelInf){
        fileHeader.idLength = setPixelInf;
        inimage.seekg(0);
        inimage.write((char*)&fileHeader, sizeof(fileHeader));
    }

    auto canEncrypt(std::string const &message) -> bool override{
        return message.size() * 8 <= pow(2,8)|| message.size() * 8 <=fileHeader.width*fileHeader.height;
    }

    auto info()->void override{
        auto timeRaw = std::filesystem::last_write_time(std::filesystem::path(path));
        auto clock = std::chrono::clock_cast<std::chrono::system_clock>(timeRaw);
        auto timeT = std::chrono::system_clock::to_time_t(clock);
        fmt::println("Width: {}\nHeight: {}\nSize: {}kB\nLast modification time: {}",fileHeader.width,fileHeader.height,size/1024,std::ctime(&timeT));
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
        for (int i = 1; i <= fileHeader.idLength; i++) {
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