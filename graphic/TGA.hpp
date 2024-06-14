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
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imageType;
    uint16_t colorMapOrigin;
    uint16_t colorMapLength;
    uint8_t colorMapDepth;
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t bitDepth;
    uint8_t imageDescriptor;
};
#pragma pack(pop)


struct TGA: ImageAbstract{
    TARGAFileHeader fileHeader;
    std::basic_fstream<char> inimage;
    std::string path;
    int size;
    //oblicznie rozmiaru w zaleznosci od parametrow headera
    int calculateTGAFileSize(const TARGAFileHeader &header) {
        int headerSize = 18;
        int idLength = header.idLength;
        int colorMapSize = header.colorMapType ? header.colorMapLength * (header.colorMapDepth / 8) : 0;
        int pixelDataSize = header.width * header.height * (header.bitDepth / 8);
        return headerSize + idLength + colorMapSize + pixelDataSize;
    }

    TGA(std::string const &path) {
        this -> path = path;
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!inimage) {
            fmt::println("Can't open this file!");
        }
        inimage.read((char*)&fileHeader, sizeof(fileHeader));
        size = calculateTGAFileSize(fileHeader);
    }
    //pierwsze 16 bajtów przekazywane na rzecz zapisania ilosci potrzebnych bajtow przy odczycie
    auto setPixels(std::fstream &stream , int numberOfBits){
        auto bitNum = std::bitset<16>(numberOfBits);
        for(int i=0;i<16;i++){
            auto bit = char();
            stream.read(&bit, 1);
            bit &= ~1;
            if (bitNum[15]) bit |= 1;
            bitNum<<=1;
            stream.seekp(-1, std::ios::cur);
            stream.write(&bit,1);
        }
        stream.peek();
    }
    //czytanie pierwszywch 16 bajtow
    auto readPixels(std::fstream &stream) -> int{
        auto charBit = std::string();
        for (int i = 0; i < 16; i++) {
            auto bit = char();
            stream.read(&bit, 1);
            charBit += std::to_string(std::bitset<8>(bit)[0]);
        };
        return std::stoi(charBit, nullptr,2);
    }


    auto canEncrypt(std::string const &message) -> bool override{
        return (message.size() * 8 <= pow(2,16)) || (message.size() * 8 <=fileHeader.width*fileHeader.height-16);
    }

    auto info()->void override{
        auto timeT = getFileTime(path);
        fmt::println("Format: TGA\nWidth: {}\nHeight: {}\nSize: {}kB\nLast modification time: {}",
                     fileHeader.width,fileHeader.height,size/1024,std::ctime(&timeT));
    }


    auto encryptMessage(std::string &message) -> void override{
        auto pixelMessage = (message.size() * 8);
        if(!canEncrypt(message)){
            fmt::println("Sorry, but you can't encypt this message in this file");
            return;
        }
        setPixels(inimage,pixelMessage);
        encryptMessageStatic(message, inimage);
    }
    auto decryptMessage() -> void override{
        auto toRead = readPixels(inimage);
        decryptMessageStatic(toRead,inimage);
    }
};