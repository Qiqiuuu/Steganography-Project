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
    int width;
    int height;
    int size;

    BMP(std::string const &path) {
        inimage = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!inimage) {
            fmt::println("Can't open this file!");
            return;
        }
        auto bmpHeader = std::vector<char>(54);
        inimage.read(bmpHeader.data(), bmpHeader.size());
        width = *(int *) &bmpHeader[18];
        height = *(int *) &bmpHeader[22];
        size = *(int *) &bmpHeader[4];
    }

    auto checkIfCanEncode(int const &amountOfPixels) -> bool {
        return amountOfPixels <= (width * height) - width;
    }

    auto encodeMessage(std::string &message) -> void {
        std::cout << message << "\n";
        auto pixelMessage = (message.size() * 8) / 2;
        fmt::println("ilosc pikseli zajetych: {}", pixelMessage);
        auto pixVec = std::vector<int>();
        auto k = pixelMessage;
        while (k > 0) {
            pixVec.push_back(k % 10);
            k /= 10;
        }
        std::reverse(pixVec.begin(), pixVec.end());
        for (int i = 0; i < width; i += 2) {
            char r1, g1, b1, r2, g2, b2;
            inimage.read((char *) (&r1), 1);
            inimage.read((char *) (&g1), 1);
            inimage.read((char *) (&b1), 1);
            inimage.read((char *) (&r2), 1);
            inimage.read((char *) (&g2), 1);
            inimage.read((char *) (&b2), 1);
            r1 &= ~1;
            g1 &= ~1;
            b1 &= ~1;
            r2 &= ~1;
            g2 &= ~1;
            b2 &= ~1;
            if (i / 2 < pixVec.size()) {
                auto number = std::bitset<6>(pixVec[i / 2]);
                for (int j = 0; j < 6; ++j) {
                    bool lsb = number[0];
                    number >>= 1;
                    switch (j) {
                        case 0:
                            if (lsb) b2 |= 1;
                            break;
                        case 1:
                            if (lsb) g2 |= 1;
                            break;
                        case 2:
                            if (lsb) r2 |= 1;
                            break;
                        case 3:
                            if (lsb) b1 |= 1;
                            break;
                        case 4:
                            if (lsb) g1 |= 1;
                            break;
                        case 5:
                            if (lsb) r1 |= 1;
                            break;
                    }
                }
            }
            inimage.seekp(-6, std::ios::cur);
            inimage.write((char *) (&r1), 1);
            inimage.write((char *) (&g1), 1);
            inimage.write((char *) (&b1), 1);
            inimage.write((char *) (&r2), 1);
            inimage.write((char *) (&g2), 1);
            inimage.write((char *) (&b2), 1);

        }
        auto bitIndex = 4;
        auto charVec = std::vector<std::bitset<8>>();
        for (char c: message) {
            charVec.push_back(std::bitset<8>(c));
        }
        for(int i=0;i<pixelMessage;i++){
            char r, g, b;
            inimage.read((char *) (&r), 1);
            inimage.read((char *) (&g), 1);
            inimage.read((char *) (&b), 1);
            std::cout<<"==="<<charVec[i/4]<<"===";
            r &= ~1;
            b &= ~1;
            if ((bool)charVec[i/4][7]) r |= 1;
            if ((bool)charVec[i/4][6]) b |= 1;
            charVec[i/4]<<=2;
            std::cout<<std::to_string(std::bitset<8>(r)[0])<<std::to_string(std::bitset<8>(b)[0]);
            inimage.seekp(-3, std::ios::cur);
            inimage.write((char *) (&r), 1);
            inimage.write((char *) (&g), 1);
            inimage.write((char *) (&b), 1);
        }
    }


    auto decodeMessage() {
        auto pixels = std::string();
        for (int i = 0; i < width; i += 2) {
            char r1, g1, b1, r2, g2, b2;
            inimage.read((char *) (&r1), 1);
            inimage.read((char *) (&g1), 1);
            inimage.read((char *) (&b1), 1);
            inimage.read((char *) (&r2), 1);
            inimage.read((char *) (&g2), 1);
            inimage.read((char *) (&b2), 1);
            auto number = std::string();
            number += std::to_string(std::bitset<8>(r1)[0]);
            number += std::to_string(std::bitset<8>(g1)[0]);
            number += std::to_string(std::bitset<8>(b1)[0]);
            number += std::to_string(std::bitset<8>(r2)[0]);
            number += std::to_string(std::bitset<8>(g2)[0]);
            number += std::to_string(std::bitset<8>(b2)[0]);
            auto intPixel = std::bitset<6>(number);
            if (intPixel.to_ulong() != 0)
                pixels += std::to_string(intPixel.to_ulong());
        }
        fmt::println("{}             ", pixels);

        auto message = std::string();
        auto charBit = std::string();
        for (int i = 1; i <= std::stoi(pixels); i++) {
            char r, g, b;
            inimage.read((char *) (&r), 1);
            inimage.read((char *) (&g), 1);
            inimage.read((char *) (&b), 1);
            charBit += std::to_string(std::bitset<8>(r)[0]);
            charBit += std::to_string(std::bitset<8>(b)[0]);
            if (i%4==0) {
                message += (char)std::bitset<8>(charBit).to_ulong();
                charBit = "";
            }
            std::cout<<std::to_string(std::bitset<8>(r)[0])<<std::to_string(std::bitset<8>(b)[0]);
        };
        fmt::println("    {}", message);
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
}
auto decodeImage(std::string const& path)->void{
    auto img = BMP(path);
    img.decodeMessage();
}
auto checkIfValidExtension(std::filesystem::path const& path)->bool{
    auto extension = path.extension().string();
    return extension == ".ppm" || extension == ".bmp";
}
