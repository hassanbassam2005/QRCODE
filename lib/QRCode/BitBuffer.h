#ifndef BITBUFFER_H
#define BITBUFFER_H

#include <vector>
#include <stdexcept>
#include <cmath>

namespace QR
{
    struct BITBUFFER : public std::vector<bool>
    {
    public:
        BITBUFFER() : std::vector<bool>() {}

        static bool BINARY_BITS(long value, int length)
        {
            return ((value >> length) & 1) != 0;
        }

        void APPEND_BITS(std::uint32_t value, int length)
        {
            if (length < 0 || length > 31 || value >> length != 0)
                throw std::domain_error("Value out of range");

            for (int i = length - 1; i >= 0; i--)
            {
                this->push_back(BINARY_BITS(value, i));
            }
        }

        void APPEND_BITS(std::uint32_t value)
        {
            int length = static_cast<int>(std::log2(value) + 1);

            if (length < 0 || length > 31)
                throw std::domain_error("Value out of range");

            for (int i = length - 1; i >= 0; i--)
            {
                this->push_back(((value >> i) & 1) != 0);
            }
        }

    };
}

#endif
