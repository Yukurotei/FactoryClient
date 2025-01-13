#include <iostream>
#include <iomanip>
#include <cstring>

class SHA1 {
public:
    SHA1() { init(); }
    void update(const unsigned char* data, size_t len);
    void final(unsigned char* hash);

private:
    void init();
    void process_block();
    void pad_block();
    void transform();

    uint32_t m_data[80];
    uint32_t m_state[5];
    uint32_t m_block[16];
    size_t m_data_length;
    uint64_t m_bit_length;
};

void SHA1::init() {
    m_state[0] = 0x67452301;
    m_state[1] = 0xEFCDAB89;
    m_state[2] = 0x98BADCFE;
    m_state[3] = 0x10325476;
    m_state[4] = 0xC3D2E1F0;
    m_data_length = 0;
    m_bit_length = 0;
}

void SHA1::update(const unsigned char* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        m_data[m_data_length / 4] = (m_data[m_data_length / 4] << 8) | data[i];
        m_data_length++;
        if (m_data_length == 64) {
            process_block();
        }
    }
}

void SHA1::process_block() {
    for (size_t i = 0; i < 16; ++i) {
        m_block[i] = m_data[i];
    }
    for (size_t i = 16; i < 80; ++i) {
        m_block[i] = (m_block[i - 3] ^ m_block[i - 8] ^ m_block[i - 14] ^ m_block[i - 16]);
        m_block[i] = (m_block[i] << 1) | (m_block[i] >> 31);
    }

    uint32_t a = m_state[0];
    uint32_t b = m_state[1];
    uint32_t c = m_state[2];
    uint32_t d = m_state[3];
    uint32_t e = m_state[4];

    for (size_t i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20) {
            f = (b & c) | (~b & d);
            k = 0x5A827999;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        uint32_t temp = (a << 5) | (a >> 27);
        temp += f + e + k + m_block[i];
        e = d;
        d = c;
        c = (b << 30) | (b >> 2);
        b = a;
        a = temp;
    }

    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;
    m_state[4] += e;

    m_data_length = 0;
}

void SHA1::final(unsigned char* hash) {
    size_t len = m_data_length;
    pad_block();
    for (size_t i = 0; i < 5; ++i) {
        hash[i * 4] = (m_state[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (m_state[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (m_state[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = m_state[i] & 0xFF;
    }
}

void SHA1::pad_block() {
    uint64_t bit_length = m_bit_length + m_data_length * 8;
    m_data[m_data_length / 4] = 0x80;
    m_data_length++;
    if (m_data_length > 56) {
        process_block();
    }
    m_data[14] = bit_length >> 32;
    m_data[15] = bit_length & 0xFFFFFFFF;
    process_block();
}