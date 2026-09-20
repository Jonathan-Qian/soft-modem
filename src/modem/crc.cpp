#include <iostream>
#include <vector>
#include "crc_engine.hpp"

int main() {
    std::vector<uint8_t> frame = {0x12, 0x34, 0x56, 0x78};

    uint32_t crc24a = modem::crc::compute(modem::crc::CRC24A, frame.data(), frame.size());
    uint16_t crc16  = modem::crc::compute(modem::crc::CRC16, frame.data(), frame.size());

    std::cout << "CRC-24A: 0x" << std::hex << crc24a << "\n";
    std::cout << "CRC-16:  0x" << std::hex << crc16 << "\n";

    return 0;
}