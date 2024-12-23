#ifndef QRENCODE_H
#define QRENCODE_H

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cassert>

#include"BitBuffer.h"

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
            /**
             * @brief Constructor to initialize the mode indicator and character count bits
             * for the 3 QR code version groups.
             *
             * @param mode The mode indicator value.
             * @param charcount1 Character count bits for version group 1.
             * @param charcount2 Character count bits for version group 2.
             * @param charcount3 Character count bits for version group 3.
             */
            MODE(int mode, int charcount1, int charcount2, int charcount3)
                : MODE_INDICATOR(mode)
            {
                charCountBits[0] = charcount1;
                charCountBits[1] = charcount2;
                charCountBits[2] = charcount3;
            }

            /**
            * @brief Predefined constant static instance representing Numeric mode (0x1).
            */
            static const MODE NUMERIC;

            /**
            * @brief Predefined constant static instance representing Alphanumeric mode (0x2).
            */
            static const MODE ALPHANUMERIC;

            /**
            * @brief Predefined constant static instance representing Byte mode (0x4).
            */
            static const MODE BYTE;

            /**
            * @brief Predefined constant static instance representing Kanji mode (0x8).
            */
            static const MODE KANJI;

            /**
            * @brief Predefined constant static instance representing ECI mode.
            */
            static const MODE ECI;

            /**
            * @brief Checks if the given input string consists only of alphanumeric characters.
            *
            * @param input The input string to check.
            * @return True if the input is alphanumeric, false otherwise.
            */
            static bool IS_ALPHANUMERIC(const char* input);

            /**
            * @brief Checks if the given input string consists only of numeric characters.
            *
            * @param input The input string to check.
            * @return True if the input is numeric, false otherwise.
            */
            static bool IS_NUMERIC(const char* input);

            /**
            * @brief Returns the number of bits required to represent the mode indicator for QR code segments.
            *
            * @return The number of bits used for the mode indicator.
            */
            int MODE_BITS() const;

            /**
            * @brief Returns the number of bits required to represent the character count for
            * a given encoding mode in a specific QR code version.
            *
            * @param version The version of the QR code.
            * @return The number of bits used for the character count in the specified version.
            */
            int CHAR_COUNTER_BITS(int version) const;

            /**
            * @brief Converts a numeric input to its binary representation.
            *
            * @param input The input string containing numeric data.
            * @return An ENCODE object containing the binary representation of the numeric input.
            */
            static ENCODE NUMERIC_TO_BINARY(const char* input);

            /**
            * @brief Converts an alphanumeric input to its binary representation.
            *
            * @param input The input string containing alphanumeric data.
            * @return An ENCODE object containing the binary representation of the alphanumeric input.
            */
            static ENCODE ALPHANUMERIC_TO_BINARY(const char* input);

            /**
            * @brief Converts a byte input to its binary representation.
            *
            * @param input A vector of 8-bit unsigned integers representing the byte data.
            * @return An ENCODE object containing the binary representation of the byte input.
            */
            static ENCODE BYTE_TO_BINARY(const std::vector<std::uint8_t>& input);

            /**
            * @brief Converts an ECI input to its binary representation.
            *
            * @param input The ECI (Extended Channel Interpretation) value.
            * @return An ENCODE object containing the binary representation of the ECI input.
            */
            static ENCODE ECI_TO_BINARY(long input);

            /**
            * @brief Chooses the appropriate mode for encoding the input string into segments.
            *
            * @param input The input string to analyze.
            * @return A vector of ENCODE objects representing the encoding segments.
            */
            static std::vector<ENCODE> MODE_CHOOSER(const char* input);

            
        };//End of MODE class


    private:
        /**
        * @brief Pointer to a constant MODE object representing the encoding mode being used.
        *
        * This allows access to mode-related properties or methods without modifying the original object.
        */
        const MODE* Mode;

        /**
        * @brief Tracks the number of bits processed or currently being managed.
        *
        * Useful for managing the encoding process and ensuring correct bit alignment.
        */
        int Bit_Counter;

        /**
        * @brief Constant vector of boolean values holding the encoded data.
        *
        * Ensures that the encoded information remains unchanged throughout the object's lifetime.
        */
        const std::vector<bool> Data;


    public:
        // Constructor that initializes the ENCODE object with a constant reference to a MODE object,
        // an integer bit counter, and a reference to a vector of boolean values.
        // The vector is passed by reference, allowing the constructor to use the existing vector
        // without making a copy, which can be more efficient.
        ENCODE(const MODE& mode, int bit_counter, std::vector<bool>& data) :
            Mode(&mode), Bit_Counter(bit_counter), Data(data)
        {
            if (bit_counter < 0)
                throw std::domain_error("Invalid value");
        }

        // Constructor that initializes the ENCODE object with a constant reference to a MODE object,
        // an integer bit counter, and an rvalue reference to a vector of boolean values.
        // This constructor uses std::move to transfer ownership of the vector, allowing the
        // constructor to take a temporary vector and avoid unnecessary copies.
        ENCODE(const MODE& mode, int bit_counter, std::vector<bool>&& data) :
            Mode(&mode), Bit_Counter(bit_counter), Data(std::move(data)) 
        {
            if (bit_counter < 0)
                throw std::domain_error("Invalid value");
        }

        // Function to retrieve a pointer to the current encoding mode.
        // Returns a pointer to a constant MODE object, which represents the encoding mode being used.
        const MODE& MODE_GETTER() const;

        // Function to retrieve the data as a BITBUFFER, which stores binary data in a vector of uint8_t.
        // Returns a BITBUFFER containing the encoded data.
        const std::vector<bool> &DATA_GETTER() const;

        // Function to retrieve the size of the encoded data.
        // Returns the size of the encoded data as a size_t value.
        int SIZE_GETTER() const;

        // Function to calculate the total number of bits required for encoding the given segments.
        // Parameters:
        // - segments: A vector of ENCODE objects representing the segments to be encoded.
        // - version: An integer representing the QR code version (determines the encoding parameters).
        // Returns the total number of bits required as an integer.
        static int GET_TOTAL_BITS(const std::vector<ENCODE>& segments, int version);

    };
} // End of QR namespace

const QR::ENCODE::MODE& QR::ENCODE::MODE_GETTER() const
{   
    return* Mode;
}

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

int QR::ENCODE::MODE::MODE_BITS() const
{
    return MODE_INDICATOR;
}

int QR::ENCODE::MODE::CHAR_COUNTER_BITS(int version) const
{
    return charCountBits[(version + 7) /17];
}

QR::ENCODE QR::ENCODE::MODE::NUMERIC_TO_BINARY(const char* input)
{
    int bit_counter = 0;
    int counter = 0;
    int datas = 0;

    BITBUFFER bit;

    for (; *input != '\0'; input++,bit_counter++)
    {
        char c = *input;

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
    
    return ENCODE(NUMERIC,bit_counter, std::move(bit));
}


QR::ENCODE QR::ENCODE::MODE::ALPHANUMERIC_TO_BINARY(const char* input)
{
    int counter = 0;

    BITBUFFER bb;
   
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
    if (accumCount > 0)
        bb.APPEND_BITS(static_cast<std::uint32_t>(accumData), (accumCount == 1) ? 4 : 6);
    return ENCODE(MODE::NUMERIC, charCount, std::move(bb));
}


QR::ENCODE QR::ENCODE::MODE::BYTE_TO_BINARY(const std::vector<std::uint8_t>& input)
{
    BITBUFFER bit;

    for (std::uint8_t b : input)
    {
        bit.APPEND_BITS(b,8);
    }

    return ENCODE(MODE::BYTE, static_cast<int>(input.size()), std::move(bit));
}

QR::ENCODE QR::ENCODE::MODE::ECI_TO_BINARY(long input)
{
    BITBUFFER bit;

    if (input < 0)
        throw std::domain_error("ECI value is invalid");
    else if (input < (1 << 7))
        bit.APPEND_BITS(static_cast<std::int32_t>(input), 8);

    else if (input < (1 << 14))
    {
        bit.APPEND_BITS(2, 2);
        bit.APPEND_BITS(static_cast<std::int32_t>(input), 14);
    }

    else if (input < 1000000L)
    {
        bit.APPEND_BITS(6, 3);
        bit.APPEND_BITS(static_cast<std::int32_t>(input), 21);
    }
    else
        throw std::domain_error("ECI value is invalid");
    return ENCODE(MODE::ECI, 0, std::move(bit));
}


std::vector<QR::ENCODE> QR::ENCODE::MODE::MODE_CHOOSER(const char* input)
{
    std::vector<ENCODE> Chooser;

    if (*input == '\0') throw std::domain_error("Invalid value");
    else if (IS_NUMERIC(input))      Chooser.push_back(NUMERIC_TO_BINARY(input));
    else if (IS_ALPHANUMERIC(input)) Chooser.push_back(ALPHANUMERIC_TO_BINARY(input));
    else
    {
        std::vector<std::uint8_t> bytes;
        for (; *input != '\0'; input++)
            bytes.push_back(static_cast<std::uint8_t>(*input));
        Chooser.push_back(BYTE_TO_BINARY(bytes));
    }
                                     
   return Chooser;
}

inline const std::vector<bool> &QR::ENCODE::DATA_GETTER() const
{
    return Data;
}

inline int QR::ENCODE::SIZE_GETTER() const
{
    return Bit_Counter;
}

inline int QR::ENCODE::GET_TOTAL_BITS(const std::vector<ENCODE>& segments, int version)
{
    int result = 0;

    for (const ENCODE& segs : segments)
    {
        int ccbits = segs.Mode->CHAR_COUNTER_BITS(version);

        if (segs.Bit_Counter >= (1L << ccbits))
            return -1; 

        if (4 + ccbits > INT_MAX - result)
            return -1; 

        result += 4 + ccbits;

        if (segs.Data.size() > static_cast<unsigned long>(INT_MAX - result))
            return -1;

        result += static_cast<int>(segs.Data.size());
    }
    return result;
}

#endif
