#ifndef BITBUFFER_H
#define BITBUFFER_H

#include <vector>
#include <stdexcept>
#include <cmath>

namespace QR
{
    // Structure BITBUFFER encapsulates a bit buffer that allows for appending bits.
    struct BITBUFFER : public std::vector<bool>
    {
    public:
        // Constructor that initializes the BITBUFFER with a reference to the provided buffer.
        BITBUFFER() : std::vector<bool>() {}

        static bool BINARY_BITS(long value, int length)
        {
            return ((value >> length) & 1) != 0;
        }

        // Appends a specified number of bits from a given integer value to the BIT_BUFFER.
        void APPEND_BITS(std::uint32_t value, int length)
        {
            // Validate that the specified length is within valid bounds (0 to 31).
            if (length < 0 || length > 31 || value >> length != 0)
                throw std::domain_error("Value out of range");

            // Append each bit of the value to the BIT_BUFFER, starting from the most significant bit.
            for (int i = length - 1; i >= 0; i--)
            {
                // Shift the value right by i and mask with 1 to extract the bit, then store it in the BIT_BUFFER.
                this->push_back(BINARY_BITS(value, i));
            }
        }

        // Overloaded method to append bits from a value without needing to specify the length.
        void APPEND_BITS(std::uint32_t value)
        {
            // Calculate the minimum number of bits required to represent the value.
            int length = static_cast<int>(std::log2(value) + 1);

            // Validate that the calculated length is within valid bounds (0 to 31).
            if (length < 0 || length > 31)
                throw std::domain_error("Value out of range");

            // Append each bit of the value to the BIT_BUFFER, starting from the most significant bit.
            for (int i = length - 1; i >= 0; i--)
            {
                // Shift the value right by i and mask with 1 to extract the bit, then store it in the BIT_BUFFER.
                this->push_back(((value >> i) & 1) != 0);
            }
        }

    };
}

#endif
