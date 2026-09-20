#ifndef CRC_ENGINE_HPP
#define CRC_ENGINE_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <type_traits>

namespace modem::crc {

// 1. DYNAMIC TYPE MAPPER
template <size_t width>
using crc_t = typename std::conditional_t<width <= 8, uint8_t, typename std::conditional_t<width <= 16, uint16_t, uint32_t>>;

// 2. TABLE WRAPPER (Enables automatic template argument deduction)
template <size_t w>
struct CRCTable {
    static constexpr size_t width = w;
    using value_type = crc_t<w>;
    std::array<value_type, 256> data;

    constexpr value_type operator[](size_t idx) const {
        return data[idx];
    }
};

// 3. BUILD-TIME TABLE GENERATOR
template <size_t width, crc_t<width> poly>
consteval CRCTable<width> generate_table() {
    using T = crc_t<width>;
    CRCTable<width> tbl{};
    constexpr T mask = (width == 32) ? 0xFFFFFFFFU : static_cast<T>((1U << width) - 1U);
    constexpr T top_bit = static_cast<T>(1U << (width - 1));

    for (uint32_t i = 0; i < 256; i++) {
        T crc = static_cast<T>(i << (width - 8));

        for (int bit = 0; bit < 8; bit++) {
            crc = (crc & top_bit) ? static_cast<T>((crc << 1) ^ poly) & mask : static_cast<T>(crc << 1) & mask;
        }

        tbl.data[i] = crc;
    }
    
    return tbl;
}

// 4. PRE-GENERATED VALIDATED TABLES
inline constexpr auto CRC8   = generate_table<8,  0x07U>();
inline constexpr auto CRC16  = generate_table<16, 0x1021U>();
inline constexpr auto CRC24A = generate_table<24, 0x864CFBU>();
inline constexpr auto CRC24B = generate_table<24, 0x800063U>();
inline constexpr auto CRC32  = generate_table<32, 0x04C11DB7U>();

// 5. COMPUTATION ENGINE
template <size_t width>
constexpr crc_t<width> compute(const CRCTable<width>& table, const uint8_t* data, size_t length) {
    using T = crc_t<width>;
    T crc = 0;

    for (size_t i = 0; i < length; i++) {
        uint8_t index = static_cast<uint8_t>((crc >> (width - 8)) ^ data[i]);
        crc = static_cast<T>((crc << 8) ^ table[index]);
    }

    return crc;
}

} // namespace modem::crc

#endif // CRC_ENGINE_HPP