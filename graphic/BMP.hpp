#include <filesystem>
#include "fstream"
#include "graphic.hpp"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "vector"
#include "bitset"
#pragma once
#pragma pack(push, 1)

//pomysl na ogarniecie headera -> https://www.cs.put.poznan.pl/tzok/public/cawllp-03-bmp.html Yoink!
struct BMPFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
struct BMPInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)
struct BMP {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::basic_fstream<char> inimage;

    BMP(std::string const &path) {
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        //sprawdz bftype
        if (!inimage) {
            fmt::println("Can't open this file!");
            return;
        }
        inimage.read((char*)&fileHeader, sizeof(fileHeader));
        inimage.read((char*)&infoHeader, sizeof(infoHeader));
    }

    auto setHeader(int setPixelInf){
        fileHeader.bfReserved1 = setPixelInf;
        inimage.seekg(0);
        inimage.write((char*)&fileHeader, sizeof(fileHeader));
        inimage.read((char*)&infoHeader, sizeof(infoHeader));
    }

    auto checkIfCanEncode(std::string const &message) -> bool {
        return  message.size() * 8 <= pow(2,16)|| message.size() * 8 <=infoHeader.biWidth*infoHeader.biHeight;
    }

    auto info()->void {
        fmt::println("Width: {}\nHeight: {}\nSize: {}kB\nBitsPerPixel: {}\nPixelMessage: {}",infoHeader.biWidth,infoHeader.biHeight,fileHeader.bfSize/1000,infoHeader.biBitCount,fileHeader.bfReserved1);
    }


    auto encodeMessage(std::string &message) -> void {
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
        fmt::println("Encoded Correctly: {}",message);
    }
    auto decodeMessage() {
        auto message = std::string();
        auto charBit = std::string();
        for (int i = 1; i <= fileHeader.bfReserved1; i++) {
            auto bit = char();
            inimage.read((char *) (&bit), 1);
            charBit += std::to_string(std::bitset<8>(bit)[0]);
            if (i%8==0) {
                message += (char)std::bitset<8>(charBit).to_ulong();
                charBit = "";
            }
        };
        fmt::println("Decoded Correctly: {}", message);
    }
};