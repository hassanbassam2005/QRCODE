#ifndef REEDSOLOMON_H
#define REEDSOLOMON_H

#include <stdexcept>
#include <vector>
#include <cassert>

namespace QR
{
	struct REEDSOLOMON
	{
        /**
        * @brief Performs multiplication of two elements in Galois Field GF(2^8).
        *
        * This function computes the product of two elements `a` and `b` in GF(2^8)
        * using the Galois Field multiplication rules. The multiplication is performed
        * modulo the irreducible polynomial used to define the field, ensuring the result
        * remains within the valid range of GF(2^8).
        *
        * @param a The first element in GF(2^8) to be multiplied. This value must be
        *          in the range [0, 255].
        * @param b The second element in GF(2^8) to be multiplied. This value must be
        *          in the range [0, 255].
        * @return The product of `a` and `b` in GF(2^8). The result is guaranteed
        *         to be in the range [0, 255].
        *
        * @throws std::out_of_range if either `a` or `b` is out of the valid range
        *         for GF(2^8).
        */
        static std::uint8_t GF_MULTIPLY(uint8_t a, uint8_t b);

        /**
        * Computes the divisor polynomial for Reed-Solomon error correction code generation.
        * This method computes the coefficients of the generator polynomial, which is
        * used to encode the message with error correction.
        *
        * The generator polynomial is defined as the product of `(x - r^i)` for `i`
        * in the range [0, degree-1], where `r` is a generator element of GF(2^8).
        * Each coefficient in the polynomial is calculated in GF(2^8) arithmetic.
        *
        * @param a The degree of the divisor polynomial to compute. This value must
        * be in the range [1, 255].
        * @return A vector of uint8_t representing the coefficients of the divisor
        *         polynomial in GF(2^8), with the highest degree coefficient first.
        *
        * @throws std::domain_error if `a` is out of the valid range [1, 255].
        */
        static std::vector<uint8_t> COMPUTE_DIVISOR(int a);

        /**
        * @brief Computes the remainder of a polynomial division in GF(2^8).
        *
        * This function takes a vector representing the coefficients of a polynomial (data)
        * and divides it by another polynomial represented by the divisor. The coefficients
        * are treated as elements in the Galois Field GF(2^8), and the division is performed
        * using modulo 0x11D to ensure the results remain within the field.
        *
        * @param data A vector of uint8_t representing the coefficients of the dividend polynomial,
        * where the index corresponds to the degree of the term.
        * @param divisor A vector of uint8_t representing the coefficients of the divisor polynomial,
        * which must not be zero.
        *
        * @return A vector of uint8_t representing the coefficients of the remainder polynomial
        *         after performing the polynomial division. The degree of the remainder will
        *         be less than that of the divisor.
        *
        * @throws std::invalid_argument If the divisor is empty (zero polynomial).
        */
        static std::vector<std::uint8_t> COMPUTE_REMAINDER(
            const std::vector<std::uint8_t>& data,
            const std::vector<std::uint8_t>& divisor);


	};
}

std::uint8_t QR::REEDSOLOMON::GF_MULTIPLY(uint8_t x, uint8_t y)
{
    int z = 0;
    for (int i = 7; i >= 0; i--) {
        z = (z << 1) ^ ((z >> 7) * 0x11D);
        z ^= ((y >> i) & 1) * x;
    }
    assert(z >> 8 == 0);
    return static_cast<uint8_t>(z);
}

// Computes the divisor polynomial for Reed-Solomon error correction.
std::vector<std::uint8_t> QR::REEDSOLOMON::COMPUTE_DIVISOR(int a)
{
    if (a < 1 || a > 255) throw std::domain_error("out of range");

    std::vector<std::uint8_t> result(static_cast<size_t>(a));
    result.at(result.size() - 1) = 1; //Initialize the highest degree term to 1
    std::uint8_t root = 1; // Start with the generator element
    for (int i = 0; i < a; i++)
    {
        for (size_t j = 0; j < result.size(); j++)
        {
            result.at(j) = GF_MULTIPLY(result.at(j), root); // Multiply current coefficients by root
            if (j + 1 < result.size()) result.at(j) ^= result.at(j + 1); // XOR with next coefficient
        }
        root = GF_MULTIPLY(root, 0x02); // Update root for next iteration
    }
    return result; // Return the computed divisor polynomial
}


// This function computes the remainder when dividing the input `data` by the 
// `divisor` using polynomial division in the Galois Field GF(2^8), which is
// commonly used in error correction codes like Reed-Solomon.
inline std::vector<std::uint8_t> QR::REEDSOLOMON::COMPUTE_REMAINDER(
    const std::vector<std::uint8_t>& data, 
    const std::vector<std::uint8_t>& divisor)
{
    std::vector<uint8_t> result(divisor.size());
    for (uint8_t b : data) {  // Polynomial division
        uint8_t factor = b ^ result.at(0);
        result.erase(result.begin());
        result.push_back(0);
        for (size_t i = 0; i < result.size(); i++)
            result.at(i) ^= QR::REEDSOLOMON::GF_MULTIPLY(divisor.at(i), factor);
    }
    return result;
}



#endif
