#ifndef VERSION_H
#define VERSION_H

#include "../../lib/QREncode/QREncode.h"
#include <stdexcept>
#include <cassert>

namespace QR
{
	class VERSION
	{
	private:
		// Static constant 2D array to store the number of error correction codewords per block 
		// for different QR code versions (0 to 40) across various error correction levels.
		// Indices correspond to: 
		// [error correction level][version number].
		// Levels: 0 - Low, 1 - Medium, 2 - Quartile, 3 - High
		static const std::int8_t ECC_CODEWORDS_PER_BLOCK[4][41];

		// Static constant 2D array to store the number of error correction blocks 
		// for different QR code versions (0 to 40) across various error correction levels.
		// Indices correspond to: 
		// [error correction level][version number].
		// Levels: 0 - Low, 1 - Medium, 2 - Quartile, 3 - High
		static const std::int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41];

	public:
		// Minimum version number for QR codes
		const int MIN_VERSION = 1;

		// Maximum version number for QR codes
		const int MAX_VERSION = 40;

		// Enum class representing the different levels of error correction for QR codes
		enum class ERROR
		{
			LOW = 0,       // Low error correction level
			MEDIUM,       // Medium error correction level
			QUARTILE,     // Quartile error correction level
			HIGH          // High error correction level
		};

		// Function to retrieve the number of bits used for error correction based on the specified error correction level
		int GETBITSERROR(ERROR err);

		// Function to get the capacity in bits for a given QR code version
		int GET_CAPACITY_BITS(int version) const;

		// Function to get the capacity in codewords for a given QR code version and error correction level
		int GET_CAPACITY_CODEWORDS(int version, ERROR ecl) const;

	};
}

// Function to retrieve the number of bits used for error correction
// based on the specified error correction level (ERROR).
int QR::VERSION::GETBITSERROR(ERROR err)
{
	// Determine the number of bits for the given error correction level
	switch (err)
	{
	case QR::VERSION::ERROR::LOW:
		return 1;  // Return 1 bit for low error correction level
		break;
	case QR::VERSION::ERROR::MEDIUM:
		return 0;  // Return 0 bits for medium error correction level
		break;
	case QR::VERSION::ERROR::QUARTILE:
		return 2;  // Return 2 bits for quartile error correction level
		break;
	case QR::VERSION::ERROR::HIGH:
		return 3;  // Return 3 bits for high error correction level
		break;
	default:
		// Throw an exception if an unreachable case is encountered
		throw std::logic_error("Unreachable");
		break;
	}
}

int QR::VERSION::GET_CAPACITY_BITS(int version) const
{
	// Check if the provided version is within the valid range (1 to 40)
	if (version > MAX_VERSION || version < MIN_VERSION)
		throw std::domain_error("Version out of range");

	// Calculate the initial result based on the version
	int result = (16 * version + 128) * version + 64;

	// Adjust the result for versions greater than or equal to 2
	if (version >= 2)
	{
		// Calculate the number of alignment patterns based on the version
		int numAlign = version / 7 + 2;
		// Adjust result for alignment patterns
		result -= (25 * numAlign - 10) * numAlign - 55;

		// Further adjustment for versions 7 and above
		if (version >= 7)
			result -= 36;
	}

	// Assert that the result is within the expected range for capacity bits
	assert(208 <= result && result <= 29648);

	// Return the calculated capacity bits for the specified version
	return result;
}


int QR::VERSION::GET_CAPACITY_CODEWORDS(int version, ERROR ecl) const
{
	// Calculate the capacity in bits for the specified version
	// and convert it to capacity in codewords by dividing by 8.
	// Then, subtract the error correction codewords for the specified error correction level (ecl).
	// The number of error correction codewords is determined by 
	// the ECC_CODEWORDS_PER_BLOCK and NUM_ERROR_CORRECTION_BLOCKS arrays.
	return GET_CAPACITY_BITS(version) / 8
		- ECC_CODEWORDS_PER_BLOCK[static_cast<int>(ecl)][version]
		* NUM_ERROR_CORRECTION_BLOCKS[static_cast<int>(ecl)][version];
}



const int8_t QR::VERSION::ECC_CODEWORDS_PER_BLOCK[4][41] = {
	//0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
	{-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
	{-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
	{-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
};

const int8_t QR::VERSION::NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
	//0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
	{-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
	{-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
	{-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};


#endif