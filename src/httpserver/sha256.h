#ifndef SHA256_H
#define SHA256_H

#include <array>
#include <string>

class SHA256 {
public:
    static std::string hash(const std::string& data);

private:
    static const size_t block_size = 64;
    static const size_t hash_size = 32;

    static void transform(uint32_t state[8], const uint8_t block[block_size]);
    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t sigma0(uint32_t x);
    static uint32_t sigma1(uint32_t x);
    static uint32_t sum0(uint32_t x);
    static uint32_t sum1(uint32_t x);
};

#endif
