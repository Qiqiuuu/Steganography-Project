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
    auto getData()->void{
        auto testSTR = std::string();
        auto index = 0;
        while (inimage >> testSTR){
            if(testSTR[0]!='#'){
                switch (index) {
                    case 0:
                        format = testSTR;
                        break;
                    case 1:
                        width = std::stoi(testSTR);
                        break;
                    case 2:
                        height = std::stoi(testSTR);
                        break;
                    case 3:
                        maxColorValue =std::stoi(testSTR);
                        break;
                }
                if(++index>=4)break;
            }
        }
    };
    //podmienia zakomentowana ilosc pixeli na potrzebna przy dekrypcji obecnej
    //kodowanej wiadomosci
    auto setHeader(int setPixelInf)->void{
        std::string insert = "#" + std::to_string(setPixelInf) + "\n";
        auto restFile = std::stringstream();
        inimage.seekg(bitsNeeded.size(), std::ios::beg);
        restFile << inimage.rdbuf();
        inimage.seekp(0);
        inimage << insert << restFile.str();
        inimage.seekg(0);
        inimage >> bitsNeeded;
        getData();
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
        //sprawdzenie czy jest komentarz z iloscia zapisanych bitow
        //jak nie to daje 0 bazowo
        if(buffer[0]!='#'){
            bitsNeeded = "#0\n";
            auto restFile = std::stringstream();
            restFile << inimage.rdbuf();
            inimage.seekp(0);
            inimage << bitsNeeded << buffer << restFile.str();
            inimage.seekg(bitsNeeded.size(), std::ios::beg);
        }else
            bitsNeeded = buffer;
        getData();
        //zlicznie rozmiaru
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
        auto timeT = getFileTime(path);
        fmt::println("Format: PPM\nWidth: {}\nHeight: {}\nSize: {}kB\nLast modification time: {}",
                     width,height,size/1024,std::ctime(&timeT));
    }
    auto encryptMessage(std::string &message) -> void override {
        auto pixelMessage = (message.size() * 8);
        if(!canEncrypt(message)){
            fmt::println("Sorry, but you can't encypt this message in this file");
            return;
        }
        setHeader(pixelMessage);
        encryptMessageStatic(message, inimage);
    }
    auto decryptMessage() -> void override{
        decryptMessageStatic(bits, inimage);
    }
};
