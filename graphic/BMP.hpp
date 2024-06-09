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
struct BMP : ImageAbstract{
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::basic_fstream<char> inimage;
    std::string path;

    BMP(std::string const &path) {
        this -> path = path;
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!inimage) {
            fmt::println("Can't open this file!");
        }
        //wczytywanie headera do zmiennych
        inimage.read((char*)&fileHeader, sizeof(fileHeader));
        inimage.read((char*)&infoHeader, sizeof(infoHeader));
    }
    //wstawianie do headera informacji o ilosci bitow potrzebnych do zapisania wiadomosci
    auto setHeader(int setPixelInf){
        fileHeader.bfReserved1 = setPixelInf;
        inimage.seekg(0);
        inimage.write((char*)&fileHeader, sizeof(fileHeader));
        inimage.read((char*)&infoHeader, sizeof(infoHeader));
    }
    //sprawdzanie czy wiadomosci moze sie zmiescic w int lub czy obraz moze pomiescic ilosc bitow
    //dla roznych "formatow" jak 24b per pixel lub 32b pp
    auto canEncrypt(std::string const &message) -> bool override{
        return message.size() * 8 <= pow(2,16)|| message.size() * 8 <=infoHeader.biWidth*infoHeader.biHeight*(infoHeader.biBitCount<8?1:infoHeader.biBitCount/8);
    }
    //zwracane info o obrazie
    auto info()->void override{
        auto timeT = getFileTime(path);
        fmt::println("Format: BMP\nWidth: {}\nHeight: {}\nSize: {}kB\nLast modification time: {}"
                     ,infoHeader.biWidth,infoHeader.biHeight,
                     fileHeader.bfSize/1024,std::ctime(&timeT));
    }
    //encrypt dla bmp
    auto encryptMessage(std::string &message) -> void override{
        auto pixelMessage = (message.size() * 8);
        setHeader(pixelMessage);
        encryptMessageStatic(message,inimage);
    }
    //decrypt dla obrazu
    auto decryptMessage() -> void override {
        decryptMessageStatic(fileHeader.bfReserved1, inimage);
    }
};