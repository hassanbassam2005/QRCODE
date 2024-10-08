#ifndef BITBUFFER_H
#define BITBUFFER_H

#include <vector>
#include <stdexcept>
#include <cmath>


// Template class BITBUFFER parameterized by typename BUFF
template<typename BUFF>
// Structure BITBUFFER encapsulates a bit buffer that allows for appending bits.
struct BITBUFFER
{
private:
    // Reference to an instance of BUFF that serves as the underlying bit buffer.
    BUFF& BIT_BUFFER;

public:
    // Constructor that initializes the BITBUFFER with a reference to the provided buffer.
    BITBUFFER(BUFF& BUFFER) : BIT_BUFFER(BUFFER) {}

    // Appends a specified number of bits from a given integer value to the BIT_BUFFER.
    void APPEND_BITS(std::uint32_t value, int length)
    {
        // Validate that the specified length is within valid bounds (0 to 31).
        if (length < 0 || length > 31)
            throw std::domain_error("Value out of range");

        // Append each bit of the value to the BIT_BUFFER, starting from the most significant bit.
        for (int i = length - 1; i >= 0; i--)
        {
            // Shift the value right by i and mask with 1 to extract the bit, then store it in the BIT_BUFFER.
            BIT_BUFFER.push_back(((value >> i) & 1) != 0);
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
            BIT_BUFFER.push_back(((value >> i) & 1) != 0);
        }
    }

    // Getter function to return the constant reference to the bit buffer.
    const BUFF& BUFFER_GETTER() const
    {
        return BIT_BUFFER;
    }

};
#endif
