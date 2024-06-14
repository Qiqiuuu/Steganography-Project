#include <string>
#pragma once
struct ImageAbstract {
    virtual auto encryptMessage(std::string &message) -> void {};

    virtual auto decryptMessage() -> void {};

    virtual auto info() -> void {};

    virtual auto canEncrypt(std::string const &message) -> bool {};

    static auto getFileTime(std::filesystem::path const& path){
        auto timeRaw = std::filesystem::last_write_time(std::filesystem::path(path));
        auto clock = std::chrono::clock_cast<std::chrono::system_clock>(timeRaw);
        auto timeT = std::chrono::system_clock::to_time_t(clock);
        return timeT;
    }
    //sysytem kryptowania ktory przechodzi przez kolejne bajty zdjecia zmieniajac LSB na bity charów z wiadomosci
    static auto encryptMessageStatic(std::string &message,std::basic_fstream<char> &stream){
        auto pixelMessage = (message.size() * 8);
        auto charVec = std::vector<std::bitset<8>>();
        for (char c: message) {
            charVec.push_back(std::bitset<8>(c));
        }
        for(int i=0;i<pixelMessage;i++){
            auto bit = char();
            stream.read((char *) (&bit), 1);
            bit &= ~1;
            if (charVec[i/8][7]) bit |= 1;
            charVec[i/8]<<=1;
            stream.seekp(-1, std::ios::cur);
            stream.write((char *) (&bit),1);
        }
        stream.peek();
        fmt::println("Encrypted Correctly: {}\n",message);
    }
    //zczytuje przez x bajtow ostatnie bity i zamienia w wiadomosc
    static auto decryptMessageStatic(uint16_t const&amountOfBits,std::basic_fstream<char> &stream){
        auto message = std::string();
        auto charBit = std::string();
        for (int i = 1; i <= amountOfBits; i++) {
            auto bit = char();
            stream.read((char *) (&bit), 1);
            charBit += std::to_string(std::bitset<8>(bit)[0]);
            if (i%8==0) {
                message += (char)std::bitset<8>(charBit).to_ulong();
                charBit = "";
            }
        };
        fmt::println("Decrypted Correctly: {}\n", message);
    }
};