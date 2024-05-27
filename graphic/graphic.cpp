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

struct BMP {
    std::basic_fstream<char> inimage;
    std::vector<char> BMPHeader;
    int width;
    int height;
    int size;
    int pixelInformation1;
    int pixelInformation2;
    int numberOfBitsPerPixel;
    int pixelInformation;

    BMP(std::string const &path) {
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!inimage) {
            fmt::println("Can't open this file!");
            return;
        }
        BMPHeader = std::vector<char>(54);
        inimage.read(BMPHeader.data(), BMPHeader.size());
        width = *(int *)&BMPHeader[18];
        height =  *(int *)&BMPHeader[22];
        pixelInformation1 = *(int *)&BMPHeader[6];
        pixelInformation2 = *(int *)&BMPHeader[7];
        numberOfBitsPerPixel = *(int *)&BMPHeader[28];
        size = width*height*(numberOfBitsPerPixel/8)+sizeof(BMPHeader);
        std::cout<<pixelInformation;
    }

    auto setHeader(int setPixelInf){
        std::cout<<"pixels"<<setPixelInf<<"\n";
        if(setPixelInf > 255){
            BMPHeader[7] = setPixelInf>>= 8;
            BMPHeader[6] = setPixelInf>>=8;
            pixelInformation = BMPHeader[6] << 8 | BMPHeader[7];
        } else{
            BMPHeader[7] = setPixelInf;
            std::cout<<"\n"<<BMPHeader[7];
            pixelInformation = BMPHeader[7];
        }
        inimage.seekg(0);
        inimage.write(BMPHeader.data(), BMPHeader.size());

    }

    auto checkIfCanEncode(int const &amountOfPixels) -> bool {
        return amountOfPixels <= pow(2,16)||width*height*(numberOfBitsPerPixel/8);
    }

    auto info()->void {
        fmt::println("Width: {}\nHeight: {}\nSize: {}kB\nBitsPerPixel: {}\nPixelMessage: {}",width,height,size/(1024),numberOfBitsPerPixel,pixelInformation);
    }


    auto encodeMessage(std::string &message) -> void {
        auto pixelMessage = (message.size() * 8);
        fmt::println("PixelSize: {}",pixelMessage);
        setHeader(pixelMessage);
        auto charVec = std::vector<std::bitset<8>>();
        for (char c: message) {
            charVec.push_back(std::bitset<8>(c));
        }
        for(int i=0;i<pixelMessage;i++){
            auto bit = char();
            inimage.read((char *) (&bit), 1);
            bit &= ~1;
            if ((bool)charVec[i/8][7]) bit |= 1;
            charVec[i/8]<<=1;
            inimage.seekp(-1, std::ios::cur);
            inimage.write((char *) (&bit), 1);
        }
        fmt::println("Encoded Correctly: {}",message);
    }
    auto decodeMessage() {
        auto message = std::string();
        auto charBit = std::string();
        for (int i = 1; i <= pixelInformation; i++) {
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


auto encodeImage(std::string const& path,std::string &message)->void{
    auto pixelMessage = message.size()*8;
    auto img = BMP(path);
    if(!img.checkIfCanEncode(pixelMessage)) {
        fmt::println("Too long message/not enough pixels");
        return;
    }
    img.encodeMessage(message);
    img.info();
}
auto decodeImage(std::string const& path)->void{
    auto img = BMP(path);
    img.decodeMessage();
}
auto checkIfValidExtension(std::filesystem::path const& path)->bool{
    auto extension = path.extension().string();
    return extension == ".ppm" || extension == ".bmp";
}
