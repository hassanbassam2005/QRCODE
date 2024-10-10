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
        static int GF_INVERSE(int a);

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
        static std::uint8_t GF_DEVIDE(int a, int b);

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

        static std::vector<std::uint8_t> COMPUTE_REMAINDER(
            const std::vector<std::uint8_t>& data,
            const std::vector<std::uint8_t>& divisor);

	};
}

int QR::REEDSOLOMON::GF_INVERSE(int a)
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
    return static_cast<uint8_t>(result);
}

// Divides two elements in GF(2^8).
// Computes a / b using multiplication by the inverse of b.
std::uint8_t QR::REEDSOLOMON::GF_DEVIDE(int a, int b)
{
    if (b == 0) throw std::domain_error("Divided by zero");

    // Find the multiplicative inverse of b in GF(2^8)
    int inverse_b = GF_INVERSE(b);

    // Multiply a by the inverse of b in GF(2^8)
    int result = GF_MULTIPLY(inverse_b, a);

    return result;
}


// Computes the divisor polynomial for Reed-Solomon error correction.
std::vector<std::uint8_t> QR::REEDSOLOMON::COMPUTE_DIVISOR(int a)
{
    if (a < 1 || a > 255) throw std::domain_error("out of range");

    std::vector<std::uint8_t> result(static_cast<size_t>(a));
    result.back() = 1; // Initialize the highest degree term to 1
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
    // Initialize the result vector to the same size as the divisor. This will
    // store the remainder during the division process.
    std::vector<std::uint8_t> result(divisor.size());

    // Iterate over each byte of the input data.
    for (std::uint8_t b : data)
    {
        // Calculate the "factor" for the current division step.
        // The first element of the result is used as the leading coefficient
        // for the current step of polynomial division.
        std::uint8_t factor = b ^ result.at(0);

        // Shift the result to the left (simulating polynomial division).
        // The first element is removed, and a 0 is appended to the end.
        result.erase(result.begin());  // Remove the first element
        result.push_back(0);           // Append 0 to maintain the size of result

        // Now we apply the division step by multiplying each element of the divisor
        // by the factor, and then XORing it with the corresponding element in the result.
        for (size_t i = 0; i < result.size(); i++)
        {
            // Multiply the current divisor element by the factor using GF(2^8) multiplication
            // and XOR it with the current result element.
            result.at(i) ^= GF_MULTIPLY(divisor.at(i), factor);
        }
    }

    // After processing all the data bytes, the remainder is stored in the `result` vector,
    // which now contains the error correction codewords.
    return result;
}



#endif
