#ifndef QRENCODE_H
#define QRENCODE_H

#include <iostream>
#include <cstring>
#include <vector>
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

            static std::vector<ENCODE> MODE_CHOOSER(const char* input);
            
        };//End of MODE class
    private:
        // Pointer to a constant MODE object, which represents the encoding mode being used.
        // This allows access to mode-related properties or methods without modifying the original object.
        const MODE* Mode;

        // Integer that counts the number of bits processed or currently being tracked.
        // This can be useful for managing the encoding process and ensuring correct bit alignment.
        size_t Bit_Counter;
            
        // Constant vector of boolean values that holds the encoded data.
        // Using a const vector prevents modification of the data after it is set,
        // ensuring that the encoded information remains unchanged throughout the object's lifetime.
        BITBUFFER<std::vector<std::uint8_t>> Data;

    public:
        // Constructor that initializes the ENCODE object with a constant reference to a MODE object,
        // an integer bit counter, and a reference to a vector of boolean values.
        // The vector is passed by reference, allowing the constructor to use the existing vector
        // without making a copy, which can be more efficient.
        ENCODE(const MODE& mode, size_t bit_counter, BITBUFFER<std::vector<std::uint8_t>>& data) :
            Mode(&mode), Bit_Counter(bit_counter), Data(data)
        {
            if (bit_counter < 0)
                throw std::domain_error("Invalid value");
        }

        // Constructor that initializes the ENCODE object with a constant reference to a MODE object,
        // an integer bit counter, and an rvalue reference to a vector of boolean values.
        // This constructor uses std::move to transfer ownership of the vector, allowing the
        // constructor to take a temporary vector and avoid unnecessary copies.
        ENCODE(const MODE& mode, size_t bit_counter, BITBUFFER<std::vector<std::uint8_t>>&& data) :
            Mode(&mode), Bit_Counter(bit_counter), Data(std::move(data)) 
        {
            if (bit_counter < 0)
                throw std::domain_error("Invalid value");
        }

        

        const MODE* MODE_GETTER();
        BITBUFFER<std::vector<std::uint8_t>> DATA_GETTER() const;
        size_t SIZE_GETTER() const;
        
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
    int counter = 0;
    int datas = 0;
    // Vector to hold the binary representation of the input.
    std::vector<std::uint8_t> buffer;

    // Create a BITBUFFER object that manages appending bits to the buffer.
    BITBUFFER<std::vector<std::uint8_t>> bit(buffer);

    // Append the bits of the input to the BITBUFFER. 
    // The input is cast to a uint32_t to ensure proper bit representation.
    for (; *input != '\0'; input++)
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
        bit.APPEND_BITS(static_cast<std::uint32_t>(datas));
    
    
    // Create and return an ENCODE object with the encoding mode set to NUMERIC,
    // the size of the buffer, and the buffer's contents moved into the ENCODE object.
    return ENCODE(NUMERIC,buffer.size(), std::move(bit));
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
    return ENCODE(MODE::NUMERIC, buffer.size(), std::move(bb));
}


QR::ENCODE QR::ENCODE::MODE::BYTE_TO_BINARY(const std::vector<std::uint8_t>& input)
{

    int num = 0;
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

    return ENCODE(MODE::NUMERIC, buffer.size(), std::move(bit));
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
BITBUFFER<std::vector<std::uint8_t>> QR::ENCODE::DATA_GETTER() const
{
    return Data;
}

// Getter function to return the size of the bit counter.
size_t QR::ENCODE::SIZE_GETTER() const
{
    return Bit_Counter;
}

// Getter function to return the encoding mode.
const QR::ENCODE::MODE* QR::ENCODE::MODE_GETTER()
{
    return Mode;
}



#endif
