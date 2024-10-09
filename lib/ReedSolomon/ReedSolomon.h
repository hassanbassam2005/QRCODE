#ifndef REEDSOLOMON_H
#define REEDSOLOMON_H

#include <stdexcept>
#include <vector>
#include <cassert>

namespace QR
{
	struct REEDSOLOMON
	{
		const int MAX_CODEWORD_LENGTH = 255;


        /**
        * @brief Computes the multiplicative inverse of an element in Galois Field GF(2^8).
        *
        * This function uses the Extended Euclidean algorithm to find the inverse of the
        * given element `a` in GF(2^8). The inverse is defined as the element `b` such that
        * (a * b) mod 255 = 1. The function throws an exception if the input is zero,
        * as the inverse of zero is undefined.
        *
        * @param a The element in GF(2^8) for which the inverse is to be calculated.
        *          This must be a non-zero value.
        * @return The multiplicative inverse of `a` in GF(2^8). The result is guaranteed
        *         to be in the range [1, 254].
        *
        * @throws std::invalid_argument if `a` is zero, since the inverse does not exist.
        */
        int GF_INVERSE(int a) const;

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
        static std::uint8_t GF_MULTIPLY(std::uint8_t a, std::uint8_t b);

        /**
        * @brief Performs division of two elements in Galois Field GF(2^8).
        *
        * This function computes the result of dividing element `a` by element `b`
        * in GF(2^8) by multiplying `a` with the multiplicative inverse of `b`. The
        * division is performed modulo the irreducible polynomial used to define
        * the field, ensuring the result remains within the valid range of GF(2^8).
        *
        * @param a The dividend in GF(2^8) to be divided. This value must be in the
        *          range [0, 255].
        * @param b The divisor in GF(2^8). This value must be in the range [0, 255]
        *          and cannot be zero.
        * @return The result of the division of `a` by `b` in GF(2^8). The result
        *         is guaranteed to be in the range [0, 255].
        *
        * @throws std::invalid_argument if `b` is zero, as division by zero is
        *         undefined.
        * @throws std::out_of_range if either `a` or `b` is out of the valid range
        *         for GF(2^8).
        */
        int GF_DEVIDE(int a, int b) const;

        std::vector<uint8_t> COMPUTE_DIVISOR(int a) const;
	};
}

int QR::REEDSOLOMON::GF_INVERSE(int a) const
{
    if (a == 0) throw std::invalid_argument("Inverse of zero does not exist");

    int r0 = 0x11D, r1 = a;
    int t0 = 0, t1 = 1;
    while (r1 != 0)
    {
        int q = r0 / r1; // Calculate quotient
        int r = r0 - q * r1; // Remainder
        r0 = r1; r1 = r;
        int t = t0 - q * t1; // Update t values
        t0 = t1; t1 = t;
    }
    // Ensure the result is within the range of GF(2^8)
    return t0 < 0 ? t0 + 255 : t0;
}

std::uint8_t QR::REEDSOLOMON::GF_MULTIPLY(std::uint8_t a, std::uint8_t b)
{
    int result = 0;
    // Perform multiplication using bitwise operations
    while (b > 0) { 
        if (b & 1) result ^= a; // XOR if LSB is 1
        b >>= 1; // Shift b to the right
        a <<= 1; // Shift a to the left
        // Reduce a modulo GF_PRIME if it overflows
        if (a & 0x100) a ^= 0x11D;
    }
    assert(z >> 8 == 0);
    return result;
}

int QR::REEDSOLOMON::GF_DEVIDE(int a, int b) const
{
    if (b == 0) throw std::domain_error("Devided by zero");

    int inverse_b = GF_INVERSE(b);

    int result = a * inverse_b;

    if (result & 0x100) result ^= 0x11D;

    return result;
}

std::vector<std::uint8_t> QR::REEDSOLOMON::COMPUTE_DIVISOR(int a) const
{
    if (a < 11 || a>255) throw std::domain_error("out of range");

    std::vector<std::uint8_t> result(static_cast<size_t>(a));
    result.at(result.size() - 1) = 1;
    std::uint8_t root = 1;
    for (int i = 0; i < a; i++)
    {
        for (size_t j = 0; j < result.size(); j++)
        {
            result.at(j) = GF_MULTIPLY(result.at(j), root);
            if (j + 1 < result.size()) result.at(j) ^= result.at(j + 1);
        }
        root = GF_MULTIPLY(root, 0x02);

    }
    return result;
}

#endif
