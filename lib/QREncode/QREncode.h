#ifndef QRENCODE_H
#define QRENCODE_H

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cassert>
#include"../../lib/BitBuffer/BitBuffer.h"

// QR namespace that encapsulates the QR code-related functionality
namespace QR
{
    // Encode class inside QR namespace
    class ENCODE
    {
    public:
        // MODE class nested within ENCODE to represent different encoding modes (e.g., Numeric, Alphanumeric, Byte, Kanji)
        class MODE
        {
        private:
            // Mode indicator (e.g., 0x1 for Numeric, 0x2 for Alphanumeric, etc.)
            int MODE_INDICATOR;

            // Character count bits for 3 different QR code version groups
            int charCountBits[3];

        public:
            // Constructor to initialize the mode indicator and character count bits for the 3 QR code version groups
            MODE(int mode, int charcount1, int charcount2, int charcount3)
                : MODE_INDICATOR(mode)
            {
                charCountBits[0] = charcount1;
                charCountBits[1] = charcount2;
                charCountBits[2] = charcount3;
            }

            // Predefined constant static instances of MODE to represent each QR code encoding mode
            static const MODE NUMERIC;       // Numeric mode (0x1)
            static const MODE ALPHANUMERIC;  // Alphanumeric mode (0x2)
            static const MODE BYTE;          // Byte mode (0x4)
            static const MODE KANJI;         // Kanji mode (0x8)
            static const MODE ECI;
            // Function to check if the given input string consists only of alphanumeric characters (letters and digits).
            // It returns true if the input is alphanumeric, and false otherwise.
            static bool IS_ALPHANUMERIC(const char* input);

            // Function to check if the given input string consists only of numeric characters (digits).
            // It returns true if the input is numeric, and false otherwise.
            static bool IS_NUMERIC(const char* input);

            // Returns the number of bits required to represent the mode indicator for QR code segments.
            // The mode indicator specifies the encoding mode of the data (e.g., numeric, alphanumeric, byte).
            int MODE_BITS() const;

            // Returns the number of bits required to represent the character count for a given encoding mode
            // in a specific QR code version. The character count indicates how many characters are encoded
            // in the QR code and varies depending on the encoding mode and version of the QR code.
            int CHAR_COUNTER_BITS(int version) const;

            // Template function to convert a numeric input of any type to a binary representation.
            // The function takes an input of type InputType and returns an ENCODE object
            // that contains the binary representation of the numeric input.
            static ENCODE NUMERIC_TO_BINARY(const char* input);

            // Function to convert an alphanumeric input of any type to a binary representation.
            // The function takes an input of type InputType and returns an ENCODE object
            // that contains the binary representation of the alphanumeric input.
            static ENCODE ALPHANUMERIC_TO_BINARY(const char* input);

            // Function to convert a byte input of any type to a binary representation.
            // The function takes an input of type InputType and returns an ENCODE object
            // that contains the binary representation of the byte input.
            static ENCODE BYTE_TO_BINARY(const std::vector<std::uint8_t>& input);
           
            static ENCODE ECI_TO_BINARY(long input);

            static std::vector<ENCODE> MODE_CHOOSER(const char* input);
            
        };//End of MODE class

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



        }; //End of VERSION class

        class MASK
        {
        private:
            int size;
            int maskPattern;
            std::vector<std::vector<bool>> MaskMatrix;
            std::vector <std::vector<bool>> isMasked;
        public:
            void MASK_APPLY(int mask);
        };

    private:
        // Pointer to a constant MODE object, which represents the encoding mode being used.
        // This allows access to mode-related properties or methods without modifying the original object.
        const MODE* Mode;

        // Integer that counts the number of bits processed or currently being tracked.
        // This can be useful for managing the encoding process and ensuring correct bit alignment.
        int Bit_Counter;
            
        // Constant vector of boolean values that holds the encoded data.
        // Using a const vector prevents modification of the data after it is set,
        // ensuring that the encoded information remains unchanged throughout the object's lifetime.
        BITBUFFER<std::vector<std::uint8_t>> Data;

    public:
        // Constructor that initializes the ENCODE object with a constant reference to a MODE object,
        // an integer bit counter, and a reference to a vector of boolean values.
        // The vector is passed by reference, allowing the constructor to use the existing vector
        // without making a copy, which can be more efficient.
        ENCODE(const MODE& mode, int bit_counter, BITBUFFER<std::vector<std::uint8_t>>& data) :
            Mode(&mode), Bit_Counter(bit_counter), Data(data)
        {
            if (bit_counter < 0)
                throw std::domain_error("Invalid value");
        }

        // Constructor that initializes the ENCODE object with a constant reference to a MODE object,
        // an integer bit counter, and an rvalue reference to a vector of boolean values.
        // This constructor uses std::move to transfer ownership of the vector, allowing the
        // constructor to take a temporary vector and avoid unnecessary copies.
        ENCODE(const MODE& mode, int bit_counter, BITBUFFER<std::vector<std::uint8_t>>&& data) :
            Mode(&mode), Bit_Counter(bit_counter), Data(std::move(data)) 
        {
            if (bit_counter < 0)
                throw std::domain_error("Invalid value");
        }

        // Function to retrieve a pointer to the current encoding mode.
        // Returns a pointer to a constant MODE object, which represents the encoding mode being used.
        const MODE* MODE_GETTER();

        // Function to retrieve the data as a BITBUFFER, which stores binary data in a vector of uint8_t.
        // Returns a BITBUFFER containing the encoded data.
        BITBUFFER<std::vector<std::uint8_t>> DATA_GETTER() const;

        // Function to retrieve the size of the encoded data.
        // Returns the size of the encoded data as a size_t value.
        size_t SIZE_GETTER() const;

        // Function to calculate the total number of bits required for encoding the given segments.
        // Parameters:
        // - segments: A vector of ENCODE objects representing the segments to be encoded.
        // - version: An integer representing the QR code version (determines the encoding parameters).
        // Returns the total number of bits required as an integer.
        int GET_TOTAL_BITS(const std::vector<ENCODE>& segments, int version);

    };
} // End of QR namespace


// Constants
// Definition of static constant modes, initialized with their mode indicators and character count bits

// Numeric mode: mode indicator is 0x1, character count bits for version groups (10, 12, 14)
const QR::ENCODE::MODE QR::ENCODE::MODE::NUMERIC(0x1, 10, 12, 14);

// Alphanumeric mode: mode indicator is 0x2, character count bits for version groups (9, 11, 13)
const QR::ENCODE::MODE QR::ENCODE::MODE::ALPHANUMERIC(0x2, 9, 11, 13);

// Byte mode: mode indicator is 0x4, character count bits for version groups (8, 16, 16)
const QR::ENCODE::MODE QR::ENCODE::MODE::BYTE(0x4, 8, 16, 16);

// Kanji mode: mode indicator is 0x8, character count bits for version groups (8, 10, 12)
const QR::ENCODE::MODE QR::ENCODE::MODE::KANJI(0x8, 8, 10, 12);

const QR::ENCODE::MODE QR::ENCODE::MODE::ECI(0x7, 0, 0, 0);

// Alphanumeric string: This constant string contains the characters allowed in the alphanumeric mode of QR code encoding.
// The characters are listed in order of their respective index values, which are used to convert characters to binary
// when encoding an alphanumeric string into a QR code. 
// Each character is encoded as a value between 0 and 44 (inclusive), as per the QR code alphanumeric mode specification.
const char* S_ALPHANUMERIC = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

// Function to check if the given input string consists only of alphanumeric characters (letters and digits).
// Takes a C-style string 'input' as a parameter.
bool QR::ENCODE::MODE::IS_ALPHANUMERIC(const char* input)
{
    // Loop through each character in the input string until the null terminator is reached.
    for (;*input != '\0';input++)
    {
        // Check if the current character is not found in the predefined alphanumeric string (S_ALPHANUMERIC).
        // If not found, return false indicating the input is not entirely alphanumeric.
        if (!std::strchr(S_ALPHANUMERIC,*input))
            return false;
    }
    // If all characters are alphanumeric, return true.
    return true;
}

// Function to check if the given input string consists only of numeric characters (digits).
// Takes a C-style string 'input' as a parameter.
bool QR::ENCODE::MODE::IS_NUMERIC(const char* input)
{
    // Loop through each character in the input string until the null terminator is reached.
    for (const char* p = input; *p; ++p) {
        if (*p < '0' || *p > '9') return false;
    }
    return true;
}

// Returns the mode indicator for the QR code encoding.
// This value represents the specific encoding mode being used (e.g., numeric, alphanumeric, byte).
int QR::ENCODE::MODE::MODE_BITS() const
{
    return MODE_INDICATOR;
}

// Returns the number of bits required to represent the character count
// for a given QR code version. The character count is determined based
// on the encoding mode and version of the QR code. The formula calculates
// the index for the charCountBits array based on the version, adjusting for
// versions that have different bit requirements for the character count.
int QR::ENCODE::MODE::CHAR_COUNTER_BITS(int version) const
{
    return charCountBits[version < 10 ? 0 : (version < 27 ? 1 : 2)];
}

// Template function to convert a numeric input of any type to a binary representation.
// This function checks if the input is numeric, appends its bits to a buffer,
// and returns an ENCODE object with the encoding mode set to NUMERIC.
QR::ENCODE QR::ENCODE::MODE::NUMERIC_TO_BINARY(const char* input)
{
    int bit_counter = 0;
    int counter = 0;
    int datas = 0;
    // Vector to hold the binary representation of the input.
    std::vector<std::uint8_t> buffer;

    // Create a BITBUFFER object that manages appending bits to the buffer.
    BITBUFFER<std::vector<std::uint8_t>> bit(buffer);

    // Append the bits of the input to the BITBUFFER. 
    // The input is cast to a uint32_t to ensure proper bit representation.
    for (; *input != '\0'; input++,bit_counter++)
    {
        char c = *input;
        // Validate if the input is numeric. If not, throw an exception.
        if (!IS_NUMERIC(input))
            throw std::domain_error("Invalid value");

        datas = datas * 10 + (c - '0');
        counter++;
        if (counter == 3)
        {
            bit.APPEND_BITS(static_cast<std::uint32_t>(datas),10);
            counter = 0;
            datas = 0;
        }
    }

    if (counter > 0) 
        bit.APPEND_BITS(static_cast<std::uint32_t>(datas),(counter == 1) ? 4 : 7);
    
    
    // Create and return an ENCODE object with the encoding mode set to NUMERIC,
    // the size of the buffer, and the buffer's contents moved into the ENCODE object.
    return ENCODE(NUMERIC,bit_counter, std::move(bit));
}


QR::ENCODE QR::ENCODE::MODE::ALPHANUMERIC_TO_BINARY(const char* input)
{
    int counter = 0;
    // Vector to hold the binary representation of the input.
    std::vector<std::uint8_t> buffer;

    // Create a BITBUFFER object that manages appending bits to the buffer.
    BITBUFFER<std::vector<std::uint8_t>> bb(buffer);

    // Validate if the input is numeric. If not, throw an exception.
   
    int accumData = 0;
    int accumCount = 0;
    int charCount = 0;
    for (; *input != '\0'; input++, charCount++) {
        const char *temp = std::strchr(S_ALPHANUMERIC, *input);
        if (temp == nullptr)
            throw std::domain_error("String contains unencodable characters in alphanumeric mode");
        accumData = accumData * 45 + static_cast<int>(temp - S_ALPHANUMERIC);
        accumCount++;
        if (accumCount == 2) {
            bb.APPEND_BITS(static_cast<std::uint32_t>(accumData),11);
            accumData = 0;
            accumCount = 0;
        }
    }
    if (accumCount > 0)  // 1 character remaining;
        bb.APPEND_BITS(static_cast<std::uint32_t>(accumData), 6);
    // Create and return an ENCODE object with the encoding mode set to NUMERIC,
    // the size of the buffer, and the buffer's contents moved into the ENCODE object.
    return ENCODE(MODE::NUMERIC, charCount, std::move(bb));
}


QR::ENCODE QR::ENCODE::MODE::BYTE_TO_BINARY(const std::vector<std::uint8_t>& input)
{
    // Vector to hold the binary representation of the input.
    std::vector<std::uint8_t> buffer;

    // Create a BITBUFFER object that manages appending bits to the buffer.
    BITBUFFER<std::vector<std::uint8_t>> bit(buffer);

    // Append the bits of the input to the BITBUFFER. 
    // The input is cast to a uint32_t to ensure proper bit representation.
    for (std::uint8_t b : input)
    {
        bit.APPEND_BITS(b,8);
    }

    return ENCODE(MODE::NUMERIC, static_cast<int>(input.size()), std::move(bit));
}

QR::ENCODE QR::ENCODE::MODE::ECI_TO_BINARY(long input)
{
    // Create a vector to hold the binary data
    std::vector<std::uint8_t> buffer;

    // Create a BITBUFFER object to manage appending bits to the buffer
    BITBUFFER<std::vector<std::uint8_t>> bit(buffer);

    // Validate the input ECI value
    if (input < 0)
        throw std::domain_error("ECI value is invalid");

    // Handle cases based on the size of the input value
    else if (input < (1 << 7))
        // For inputs less than 128, append the input as 8 bits
        bit.APPEND_BITS(static_cast<std::int32_t>(input), 8);

    else if (input < (1 << 14))
    {
        // For inputs between 128 and 16384, append 2 bits for the mode and the input as 14 bits
        bit.APPEND_BITS(2, 2); // Mode indicator for 14-bit input
        bit.APPEND_BITS(static_cast<std::int32_t>(input), 14);
    }

    else if (input < 1000000L)
    {
        // For inputs between 16384 and 1,000,000, append 3 bits for the mode and the input as 21 bits
        bit.APPEND_BITS(6, 3); // Mode indicator for 21-bit input
        bit.APPEND_BITS(static_cast<std::int32_t>(input), 21);
    }
    else
        // If the input exceeds the valid range, throw an error
        throw std::domain_error("ECI value is invalid");

    // Return the encoded ECI value, moving the bit buffer to the ENCODE function
    return ENCODE(MODE::ECI, 0, std::move(bit));
}


std::vector<QR::ENCODE> QR::ENCODE::MODE::MODE_CHOOSER(const char* input)
{
    std::vector<ENCODE> Chooser;

    // Check for empty input and throw an exception if invalid.
    if (*input == '\0') throw std::domain_error("Invalid value");
    // If the input is numeric, convert to binary and add to the vector.
    else if (IS_NUMERIC(input))      Chooser.push_back(NUMERIC_TO_BINARY(input));
    // If the input is alphanumeric, convert to binary and add to the vector.
    else if (IS_ALPHANUMERIC(input)) Chooser.push_back(ALPHANUMERIC_TO_BINARY(input));
    // For other inputs, treat as byte and convert to binary.
    else
    {
        std::vector<std::uint8_t> bytes;
        for (; *input != '\0'; input++)
            bytes.push_back(static_cast<std::uint8_t>(*input));
        Chooser.push_back(BYTE_TO_BINARY(bytes));
    }
                                     
   // Return the vector of chosen ENCODE objects.
   return Chooser;
}



// Getter function to return the binary data buffer.
inline BITBUFFER<std::vector<std::uint8_t>> QR::ENCODE::DATA_GETTER() const
{
    return Data;
}

// Getter function to return the size of the bit counter.
inline size_t QR::ENCODE::SIZE_GETTER() const
{
    return Bit_Counter;
}

inline int QR::ENCODE::GET_TOTAL_BITS(const std::vector<ENCODE>& segments, int version)
{
    // Initialize the total bit count to zero
    int result = 0;

    // Iterate over each segment in the segments vector
    for (const ENCODE& segs : segments)
    {
        // Get the number of character counter bits for the segment's mode based on the version
        int ccbits = segs.Mode->CHAR_COUNTER_BITS(version);

        // Check if the bit counter exceeds the maximum allowable value for this mode
        if (segs.Bit_Counter >= (1L << ccbits))
            return -1; // Return -1 to indicate an error

        // Check if adding the mode indicator and character counter bits exceeds INT_MAX
        if (4 + ccbits > INT_MAX - result)
            return -1; // Return -1 to indicate an error

        // Accumulate the total bit count with mode indicator and character counter bits
        result += 4 + ccbits;

        // Ensure that the size of the segment does not exceed the remaining capacity of INT_MAX
        if (segs.SIZE_GETTER() > static_cast<unsigned int>(INT_MAX - result))
            return -1; // Return -1 to indicate an error

        // Add the size of the segment to the total bit count
        result += static_cast<int>(segs.SIZE_GETTER());
    }

    // Return the total bit count for all segments
    return result;
}


// Getter function to return the encoding mode.
const QR::ENCODE::MODE* QR::ENCODE::MODE_GETTER()
{
    return Mode;
}

// Function to retrieve the number of bits used for error correction
    // based on the specified error correction level (ERROR).
int QR::ENCODE::VERSION::GETBITSERROR(VERSION::ERROR err)
{
    // Determine the number of bits for the given error correction level
    switch (err)
    {
    case QR::ENCODE::VERSION::ERROR::LOW:
        return 1;  // Return 1 bit for low error correction level
        break;
    case QR::ENCODE::VERSION::ERROR::MEDIUM:
        return 0;  // Return 0 bits for medium error correction level
        break;
    case QR::ENCODE::VERSION::ERROR::QUARTILE:
        return 2;  // Return 2 bits for quartile error correction level
        break;
    case QR::ENCODE::VERSION::ERROR::HIGH:
        return 3;  // Return 3 bits for high error correction level
        break;
    default:
        // Throw an exception if an unreachable case is encountered
        throw std::logic_error("Unreachable");
        break;
    }
}

int QR::ENCODE::VERSION::GET_CAPACITY_BITS(int version) const
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

int QR::ENCODE::VERSION::GET_CAPACITY_CODEWORDS(int version, QR::ENCODE::VERSION::ERROR ecl) const
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


void QR::ENCODE::MASK::MASK_APPLY(int mask)
{
    if (mask < 0 || mask > 7) throw std::domain_error("Invalid mask number");

    size_t s = static_cast<size_t>(size);

    maskPattern = mask;

    for(size_t y = 0; y < s;y++)
    {
        for (size_t x = 0; x < s; x++)
        {
            bool invert = false;
            switch (maskPattern)
            {
            case 0: 
                invert = (x + y) % 2 == 0;
                break;
            case 1:
                invert = y % 2 == 0;
                break;
            case 2:
                invert = x % 3 == 0;
                break;
            case 3:
                invert = (x + y) % 3 == 0;
                break;
            case 4:
                invert = ((x / 2) + (y / 3)) % 2 == 0;
                break;
            case 5:
                invert = (x * y) % 2 + (x * y) % 3 == 0;
                break;
            case 6:
                invert = (((x + y) % 2) + ((x + y) % 3)) == 0;
                break;
            case 7:
                invert = ((x * y) % 3) % 2 == 0;
                break;
            default:
                throw std::invalid_argument("Invalid mask pattern");
            }
            if (invert) MaskMatrix[y][x] = MaskMatrix[y][x] ^ (invert & !isMasked[y][x]);
        }
    }
}


const int8_t QR::ENCODE::VERSION::ECC_CODEWORDS_PER_BLOCK[4][41] = {
    //0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
    {-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
    {-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
    {-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
    {-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
};

const int8_t QR::ENCODE::VERSION::NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
    //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
    {-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
    {-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
    {-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
    {-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};


#endif
