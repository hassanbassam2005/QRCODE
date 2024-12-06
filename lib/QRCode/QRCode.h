#ifndef QRCODE_H
#define QRCODE_H

#include "BitBuffer.h"
#include "QREncode.h"
#include "ReedSolomon.h"
#include "BitBuffer.h"

#include <sstream>
#include <array>

namespace QR
{
    class QRCODE
    {
    public:
        class VERSION
        {
        public:
            /**
            * @brief Minimum version number for QR codes.
            *
            * This constant defines the smallest version number available for QR codes.
            * Version numbers determine the size and data capacity of a QR code.
            */
            static constexpr int MIN_VERSION = 1;

            /**
             * @brief Maximum version number for QR codes.
             *
             * This constant defines the largest version number available for QR codes.
             * Version numbers range from 1 to 40, with higher versions allowing more data to be stored.
             */
            static constexpr int MAX_VERSION = 40;

            /**
             * @brief Stores the number of error correction codewords per block for each QR code version.
             *
             * This static constant 2D array contains the number of error correction codewords per block
             * for different versions of QR codes (ranging from 1 to 40) across various error correction levels.
             * Indices are structured as:
             * - Rows [0-3] correspond to error correction levels:
             *   - 0: Low (L)
             *   - 1: Medium (M)
             *   - 2: Quartile (Q)
             *   - 3: High (H)
             * - Columns [1-40] correspond to QR code versions.
             *
             * This array is used to determine the error correction capabilities of QR codes.
             */
            static const std::int8_t ECC_CODEWORDS_PER_BLOCK[4][41];

            /**
             * @brief Stores the number of error correction blocks for each QR code version.
             *
             * This static constant 2D array contains the number of error correction blocks
             * for different versions of QR codes (ranging from 1 to 40) across various error correction levels.
             * Indices are structured as:
             * - Rows [0-3] correspond to error correction levels:
             *   - 0: Low (L)
             *   - 1: Medium (M)
             *   - 2: Quartile (Q)
             *   - 3: High (H)
             * - Columns [1-40] correspond to QR code versions.
             *
             * Each entry specifies how many blocks are used for error correction in a given version
             * and error correction level, influencing the resilience of the QR code.
             */
            static const std::int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41];



            /**
            * @brief Enum class representing the different levels of error correction for QR codes.
            *
            * QR codes support four levels of error correction, allowing a trade-off between data capacity and error tolerance.
            * Each level provides a different amount of redundancy, which determines how much of the QR code can be damaged
            * while still allowing successful decoding. The levels are ordered from the lowest to the highest error correction capability.
            */
            enum class ERROR
            {
                LOW = 0,       // Low error correction level, allows for the maximum data storage but provides the least error tolerance.
                MEDIUM,        // Medium error correction level, balances data capacity and error correction capability.
                QUARTILE,      // Quartile error correction level, offers better error tolerance at the cost of reduced data capacity.
                HIGH           // High error correction level, provides the highest error tolerance but allows for the least data storage.
            };


            /**
            * @brief Retrieves the number of bits used for error correction based on the specified error correction level.
            *
            * This function returns the number of bits allocated for error correction in a QR code,
            * depending on the error correction level specified. Each error correction level
            * corresponds to a different amount of redundancy to protect against data loss.
            *
            * @param err The error correction level, specified as an `ERROR` enum value.
            *            Possible values are `ERROR::LOW`, `ERROR::MEDIUM`, `ERROR::QUARTILE`, `ERROR::HIGH`.
            * @return The number of bits used for error correction for the given level.
            */
            static int GETBITSERROR(ERROR err);

            /**
             * @brief Retrieves the capacity in bits for a given QR code version.
             *
             * This function calculates the total number of bits that can be encoded in a QR code
             * for a specific version. The version number directly affects the size of the QR code
             * and its capacity for data storage. Higher versions have larger dimensions and can
             * store more bits.
             *
             * @param version The version of the QR code, ranging from 1 to 40.
             * @return The total number of bits that can be encoded in the QR code for the specified version.
             *
             * @throws std::invalid_argument if the version is not within the range [1, 40].
             */
            static int GET_CAPACITY_BITS(int version);

            /**
             * @brief Retrieves the capacity in codewords for a given QR code version and error correction level.
             *
             * This function calculates the number of codewords (8-bit units of data) that can be stored
             * in a QR code, based on its version and the specified error correction level. The capacity
             * is reduced as the error correction level increases, due to the additional redundancy required.
             *
             * @param version The version of the QR code, ranging from 1 to 40.
             * @param ecl The error correction level, specified as an `ERROR` enum value.
             *            Possible values are `ERROR::LOW`, `ERROR::MEDIUM`, `ERROR::QUARTILE`, `ERROR::HIGH`.
             * @return The total number of codewords that can be stored in the QR code for the specified version
             *         and error correction level.
             *
             * @throws std::invalid_argument if the version is not within the range [1, 40].
             */
            static int GET_CAPACITY_CODEWORDS(int version, ERROR ecl);




        }; //End of VERSION class
    private:
        int size;
        int maskPattern;
        std::vector<std::vector<bool>> Matrix;
        std::vector <std::vector<bool>> isMasked;
        int version;
        int mask;
        QRCODE::VERSION::ERROR ErrorCorrection;
        static const int PENALTY_N1;
        static const int PENALTY_N2;
        static const int PENALTY_N3;
        static const int PENALTY_N4;
    public:
        /**
         * @brief Constructs a QRCODE object with specified version, error correction level, data codewords, and mask pattern.
         *
         * @param VERSION The version of the QR code (ranges from 1 to 40).
         * @param ECL The error correction level for the QR code.
         * @param DataCodeWords A vector containing the encoded data as codewords.
         * @param MASK The mask pattern to be applied to the QR code.
         */
        QRCODE(int VERSION,
            QR::QRCODE::VERSION::ERROR ECL,
            std::vector<std::uint8_t>& DataCodeWords,
            int MASK);

        static QRCODE ENCODE_TEXT(const char* text, QR::QRCODE::VERSION::ERROR ecl);

        static QRCODE ENCODE_BINARY(const std::vector<std::uint8_t>& data, QR::QRCODE::VERSION::ERROR ecl);

        static QRCODE ENCODE_SEGMENT(const std::vector<ENCODE>& segments, VERSION::ERROR ecl,
            int minVersion = 1,
            int maxVersion = 40,
            int mask = -1,
            bool boostEcl = true);

        /**
         * @brief Places a position marker (finder pattern) at the specified coordinates (x, y).
         *
         * Position markers are used to define the corners of the QR code, aiding in alignment
         * when scanned. They are typically found at the top-left, top-right, and bottom-left corners.
         *
         * @param x The x-coordinate of the position marker's starting point.
         * @param y The y-coordinate of the position marker's starting point.
         */
        void POSITION_MARKER(int x, int y);

        /**
         * @brief Places an alignment marker at the specified coordinates (x, y).
         *
         * Alignment markers help correct distortions in the QR code when it's being scanned.
         * They are more common in higher versions of QR codes.
         *
         * @param x The x-coordinate of the alignment marker's center.
         * @param y The y-coordinate of the alignment marker's center.
         */
        void ALIGNMENT_MARKER(int x, int y);

        /**
         * @brief Applies a specified mask pattern to the QR code.
         *
         * Masking patterns are used to ensure that the data modules are evenly distributed,
         * which helps avoid patterns that could confuse QR code readers.
         *
         * @param mask The mask pattern to apply.
         */
        void MASK_APPLY(int mask);

        /**
         * @brief Sets the color of a module (cell) at the specified coordinates (x, y).
         *
         * @param x The x-coordinate of the module.
         * @param y The y-coordinate of the module.
         * @param isColored Set to `true` to make the module black, `false` to make it white.
         */
        void SET_MODULE(int x, int y, bool isColored);

        /**
         * @brief Retrieves the color status of a module at the specified coordinates (x, y).
         *
         * @param x The x-coordinate of the module.
         * @param y The y-coordinate of the module.
         * @return `true` if the module is colored (black), `false` if it is uncolored (white).
         */
        bool MODULE(int x, int y) const;

        bool GET_MODULE(int x, int y) const;

        /**
         * @brief Gets the size of the QR code matrix.
         *
         * The size is calculated based on the version and is equal to (4 * version + 17).
         *
         * @return The size of the QR code matrix.
         */
        int SIZE_GETTER() const;

        /**
         * @brief Retrieves the version of the QR code.
         *
         * @return The version of the QR code.
         */
        int VERSION_GETTER() const;

        /**
         * @brief Retrieves the error correction level used by the QR code.
         *
         * @return The error correction level as a `QR::QRCODE::VERSION::ERROR` enum value.
         */
        QR::QRCODE::VERSION::ERROR ERROR_CORRECTION() const;



        /**
        * @brief Draws the version information pattern on the QR code.
        *
        * Version information is included for QR codes version 7 and above.
        * It contains a 6-bit code and is placed near the alignment patterns.
        * This function encodes the version number into a pattern that helps scanners
        * to identify the QR code's version.
        */
        void DRAW_VERSION();


        /**
        * @brief Draws the format bits on the QR code based on the specified mask pattern.
        *
        * The format bits encode the error correction level and the mask pattern used for the QR code.
        * These bits are crucial for QR code readers to decode the data correctly and are placed
        * near the position markers.
        *
        * @param mask The mask pattern to be encoded in the format bits.
        */
        void DRAW_FORMAT_BITS(int mask);


        /**
        * @brief Retrieves the alignment pattern positions for the current QR code version.
        *
        * Alignment patterns are used to ensure proper scanning of the QR code, especially
        * in larger versions where more precise alignment is needed. This function returns
        * a vector of integers representing the positions of alignment patterns along the
        * x and y axes.
        *
        * @return A vector of integers containing the positions of alignment patterns.
        */
        std::vector<int> ALIGMNET_PATTERN_GETTER() const;



        /**
        * @brief Adds error correction codewords and interleaves them with data codewords for a QR code.
        *
        * This function takes a vector of data codewords and calculates the error correction codewords
        * based on the specified error correction level. It then interleaves the data and error correction
        * codewords according to the QR code specification, ensuring the output is ready for matrix placement.
        *
        * @param data A constant reference to a `std::vector<std::uint8_t>` containing the data codewords
        *             to be encoded in the QR code.
        * @return A `std::vector<std::uint8_t>` containing the interleaved data and error correction codewords.
        *         The returned vector includes both the original data and the computed error correction
        *         codewords, arranged according to the QR code structure.
        */
        std::vector<std::uint8_t> ADD_ECC_INTER(const std::vector<std::uint8_t>& data) const;

        /**
        * @brief Prints the current mask pattern matrix of the QR code.
        *
        * This function outputs the mask pattern applied to the QR code,
        * showing which modules (cells) are masked or unmasked. It is primarily
        * used for debugging and visualization purposes to ensure the mask has
        * been applied correctly.
        */
        void printMask();


        /**
        * @brief Draws all functional patterns onto the QR code matrix.
        *
        * This function handles the placement of various functional patterns
        * that are required for a QR code to be scannable. These include position
        * markers, alignment patterns, timing patterns, and other necessary structures
        * like the dark module and reserved areas for format and version information.
        */
        void DRAW_FUNCTIONS();

        void DRAW_CODEWORDS(const std::vector<std::uint8_t>& data);

        int PINALTY_COUNT_PATTERNS(const std::array<int, 7>& runHistory) const;

        int PINALTY_TERMINATE_COUNT(bool currentRunColor, int currentRunLength, std::array<int, 7>& runHistory) const;

        void PENALTY_ADD_HISTORY(int currentRunLength, std::array<int, 7>& runHistory) const;

        long GET_PENALY_SCORE() const;

        const std::vector<std::vector<bool>> MATRIX_GETTER() const;
        
    };

    class data_too_long : public std::length_error
    {
    public:
        explicit data_too_long(const std::string& message);
    };
}

QR::QRCODE::QRCODE(int VERSION,
    QR::QRCODE::VERSION::ERROR ECL,
    std::vector<std::uint8_t>& DataCodeWords,
    int MASK)
    : version(VERSION), ErrorCorrection(ECL)
{
    if (VERSION < 1 || VERSION > 40)
        throw std::domain_error("value out of range");
    if (MASK < -1 || MASK > 7)
        throw std::domain_error("value out of range");
    size = VERSION * 4 + 17;

    size_t sz = static_cast<size_t>(size);

    Matrix = std::vector<std::vector<bool>>(sz, std::vector<bool>(sz));
    isMasked = std::vector<std::vector<bool>>(sz, std::vector<bool>(sz));

    DRAW_FUNCTIONS();
    const std::vector<std::uint8_t> allcodewords = ADD_ECC_INTER(DataCodeWords);
    DRAW_CODEWORDS(allcodewords);

    // Do masking
    if (MASK == -1) {  // Automatically choose best mask
        long minPenalty = LONG_MAX;
        for (int i = 0; i < 8; i++) {
            MASK_APPLY(i);
            DRAW_FORMAT_BITS(i);
            long penalty = GET_PENALY_SCORE();
            if (penalty < minPenalty) {
                MASK = i;
                minPenalty = penalty;
            }
            MASK_APPLY(i);  // Undoes the mask due to XOR
        }
    }
    assert(0 <= MASK && MASK <= 7);
    mask = MASK;
    MASK_APPLY(MASK);  // Apply the final choice of mask
    DRAW_FORMAT_BITS(MASK);  // Overwrite old format bits

    isMasked.clear();
    isMasked.shrink_to_fit();

}

inline QR::QRCODE QR::QRCODE::ENCODE_TEXT(const char* text, QR::QRCODE::VERSION::ERROR ecl)
{
    std::vector<ENCODE> segments = ENCODE::MODE::MODE_CHOOSER(text);
    return ENCODE_SEGMENT(segments, ecl);
}

inline QR::QRCODE QR::QRCODE::ENCODE_BINARY(const std::vector<std::uint8_t>& data, QR::QRCODE::VERSION::ERROR ecl)
{
    std::vector<ENCODE> segments{ ENCODE::MODE::BYTE_TO_BINARY(data) };
    return ENCODE_SEGMENT(segments, ecl);
}

inline QR::QRCODE QR::QRCODE::ENCODE_SEGMENT(const std::vector<ENCODE>& segments, VERSION::ERROR ecl,
    int minVersion,
    int maxVersion,
    int msk,
    bool boostEcl)
{
    if (!(VERSION::MIN_VERSION <= minVersion && VERSION::MAX_VERSION >= maxVersion && minVersion <= maxVersion) ||
        msk < -1 || msk > 7)
    {
        throw std::invalid_argument("Invalid value");
    }

    int version, dataUseBits;
    for (version = minVersion;; version++)
    {
        int dataCapacity = QRCODE::VERSION::GET_CAPACITY_CODEWORDS(version, ecl) * 8;
        dataUseBits = QR::ENCODE::GET_TOTAL_BITS(segments, version);
        if (dataUseBits != -1 && dataUseBits <= dataCapacity)
            break;
        if (version >= maxVersion)
        {
            std::ostringstream sb;
            if (dataUseBits == -1)
                sb << "segment too long";
            else
            {
                sb << "Data length = " << dataUseBits << " bits";
                sb << "Max capacity = " << dataCapacity << " bits";
            }
            throw data_too_long(sb.str());
        }
    }
    assert(dataUseBits != -1);

    for (VERSION::ERROR newEcl : {VERSION::ERROR::MEDIUM, VERSION::ERROR::QUARTILE, VERSION::ERROR::HIGH})
    {
        if (boostEcl && dataUseBits <= QR::QRCODE::VERSION::GET_CAPACITY_CODEWORDS(version, newEcl) * 8)
            ecl = newEcl;
    }

    BITBUFFER buffer;

    for (const ENCODE& moder : segments)
    {
        buffer.APPEND_BITS(static_cast<uint32_t>(moder.MODE_GETTER().MODE_BITS()), 4);
        buffer.APPEND_BITS(static_cast<uint32_t>(moder.SIZE_GETTER()),
            moder.MODE_GETTER().CHAR_COUNTER_BITS(version));
        buffer.insert(buffer.end(), moder.DATA_GETTER().begin(),
            moder.DATA_GETTER().end());
    }
    assert(buffer.size() == static_cast<unsigned int>(dataUseBits));
    size_t data_capacity = static_cast<size_t>(QRCODE::VERSION::GET_CAPACITY_CODEWORDS(version, ecl)) * 8;
    buffer.APPEND_BITS(0, std::min(4, static_cast<int>(data_capacity - buffer.size())));
    buffer.APPEND_BITS(0, (8 - static_cast<int>(buffer.size() % 8)) % 8);

    for (std::uint8_t pad_byte = 0xEC; buffer.size() < data_capacity; pad_byte ^= 0xEC ^ 0x11)
        buffer.APPEND_BITS(pad_byte, 8);

    std::vector<std::uint8_t> dataCodeWord(buffer.size() / 8);
    for (size_t i = 0; i < buffer.size(); i++)
        dataCodeWord.at(i >> 3) |= (buffer.at(i) ? 1 : 0) << (7 - (i & 7));

    return QRCODE(version, ecl, dataCodeWord, msk);
}


// Function to retrieve the number of bits used for error correction
// based on the specified error correction level (ERROR).
int QR::QRCODE::VERSION::GETBITSERROR(VERSION::ERROR err)
{
    // Determine the number of bits for the given error correction level
    switch (err)
    {
    case QR::QRCODE::VERSION::ERROR::LOW:
        return 1;  // Return 1 bit for low error correction level
    case QR::QRCODE::VERSION::ERROR::MEDIUM:
        return 0;  // Return 0 bits for medium error correction level
    case QR::QRCODE::VERSION::ERROR::QUARTILE:
        return 3;  // Return 2 bits for quartile error correction level
    case QR::QRCODE::VERSION::ERROR::HIGH:
        return 2;  // Return 3 bits for high error correction level
    default:
        // Throw an exception if an unreachable case is encountered
        throw std::logic_error("Unreachable");
    }
}

int QR::QRCODE::VERSION::GET_CAPACITY_BITS(int version)
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

int QR::QRCODE::VERSION::GET_CAPACITY_CODEWORDS(int version, QR::QRCODE::VERSION::ERROR ecl)
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


void QR::QRCODE::MASK_APPLY(int mask)
{
    if (mask < 0 || mask > 7) throw std::domain_error("Invalid mask number");

    size_t s = static_cast<size_t>(size);

    maskPattern = mask;

    for (size_t y = 0; y < s; y++)
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
                invert = (x / 3 + y / 2) % 2 == 0;
                break;
            case 5:
                invert = x * y % 2 + x * y % 3 == 0;
                break;
            case 6:
                invert = (x + y % 2 + x + y % 3) == 0;
                break;
            case 7:
                invert = ((x + y) % 2 + x * y % 3) % 2 == 0;
                break;
            default:
                throw std::invalid_argument("Invalid mask pattern");
            }
            Matrix[y][x] = Matrix[y][x] ^ (invert & !isMasked[y][x]);
        }
    }
}

void QR::QRCODE::printMask()
{
    for (const auto& row : Matrix) {
        for (bool val : row) {
            std::cout << (val ? "1" : "0") << " ";
        }
        std::cout << std::endl;
    }
}


// Sets a module in the QR code matrix at (x, y) with a specified color (0 or 1).
// 'isColored' determines if the module is filled (1) or empty (0).
// 'dx' and 'dy' are used as size_t to access MaskMatrix and isMasked arrays.
inline void QR::QRCODE::SET_MODULE(int x, int y, bool isColored)
{
    size_t dx = static_cast<size_t>(x);
    size_t dy = static_cast<size_t>(y);

    Matrix[dy][dx] = isColored;  // Set the color of the module in the MaskMatrix.
    isMasked[dy][dx] = true;         // Mark the module as masked.
}

// Retrieves the module state (true/false) at position (x, y) from MaskMatrix.
// Uses uint8_t casting for input parameters for consistency with array access.
inline bool QR::QRCODE::MODULE(int x, int y) const
{
    return Matrix[static_cast<size_t>(y)][static_cast<size_t>(x)];
}

inline bool QR::QRCODE::GET_MODULE(int x, int y) const
{
    return 0 <= x && x < size && 0 <= y && y < size && MODULE(x, y);
}

// Returns the size of the QR code (width/height).
inline int QR::QRCODE::SIZE_GETTER() const
{
    return size;
}

// Returns the version of the QR code.
inline int QR::QRCODE::VERSION_GETTER() const
{
    return version;
}

// Returns the error correction level used in the QR code.
inline QR::QRCODE::VERSION::ERROR QR::QRCODE::ERROR_CORRECTION() const
{
    return ErrorCorrection;
}

inline void QR::QRCODE::DRAW_VERSION()
{
    if (version < 7)
        return;

    int remainder = version;

    for (int i = 0; i < 12; i++)
    {
        remainder = (remainder << 1) ^ ((remainder >> 11) * 0x1F25);
    }

    long bits = static_cast<long>(version) << 12 | remainder;
    assert(bits >> 18 == 0);

    for (int i = 0; i < 18; i++)
    {
        bool bit = BITBUFFER::BINARY_BITS(bits, i);
        int row = size - 11 + i % 3;
        int column = i / 3;
        QR::QRCODE::SET_MODULE(row, column, bit);
        QR::QRCODE::SET_MODULE(column, row, bit);
    }
}

inline void QR::QRCODE::DRAW_FORMAT_BITS(int mask)
{
    int data = QRCODE::VERSION::GETBITSERROR(ErrorCorrection) << 3 | mask;
    int rem = data;
    for (int i = 0; i < 10; i++)
        rem = (rem << 1) ^ ((rem >> 9) * 0x537);
    int bits = (data << 10 | rem) ^ 0x5412;
    assert(bits >> 15 == 0);

    for (int i = 0; i <= 5; i++)
        SET_MODULE(8, i, ((bits >> i) & 1) != 0);

    SET_MODULE(8, 7, ((bits >> 6) & 1) != 0);
    SET_MODULE(7, 8, ((bits >> 7) & 1) != 0);
    SET_MODULE(7, 8, ((bits >> 8) & 1) != 0);

    for (int i = 9; i < 15; i++)
        SET_MODULE(14 - i, 8, ((bits >> i) & 1) != 0);

    for (int i = 0; i < 8; i++)
        SET_MODULE(size - 1 - i, 8, ((bits >> i) & 1) != 0);

    for (int i = 8; i < 15; i++)
        SET_MODULE(8, size - 15 + i, ((bits >> i) & 1) != 0);

    SET_MODULE(8, size - 8, true);
}


std::vector<int> QR::QRCODE::ALIGMNET_PATTERN_GETTER() const
{
    if (version == 1)
        return std::vector<int>();
    else
    {
        int num = version / 7 + 2;
        int step = (version * 8 + num * 3 + 5) / (num * 4 - 4) * 2;
        std::vector<int> result;
        for (int i = 0, position = size - 7; i < num - 1; i++, position -= step)
            result.insert(result.begin(), position);
        result.insert(result.begin(), 6);
        return result;
    }
}



inline std::vector<std::uint8_t> QR::QRCODE::ADD_ECC_INTER(const std::vector<std::uint8_t>& data) const
{
    if (data.size() != static_cast<unsigned int>(QR::QRCODE::VERSION::GET_CAPACITY_CODEWORDS(version, ErrorCorrection)))
        throw std::invalid_argument("Invalid argument");

    int numBlocks = QR::QRCODE::VERSION::NUM_ERROR_CORRECTION_BLOCKS[static_cast<int>(ErrorCorrection)][version];
    int blockEcc = QR::QRCODE::VERSION::ECC_CODEWORDS_PER_BLOCK[static_cast<int>(ErrorCorrection)][version];
    int rawCodeWords = QR::QRCODE::VERSION::GET_CAPACITY_BITS(version) / 8;
    int numShortBlocks = numBlocks - rawCodeWords % numBlocks;
    int shortBlockLen = rawCodeWords / numBlocks;

    std::vector<std::vector<std::uint8_t>> blocks;
    const std::vector<std::uint8_t> rsDivisor = REEDSOLOMON::COMPUTE_DIVISOR(blockEcc);

    for (int i = 0, k = 0; i < numBlocks; i++) {
        std::vector<uint8_t> dat(data.cbegin() + k, data.cbegin() + (k + shortBlockLen - blockEcc + (i < numShortBlocks ? 0 : 1)));
        k += static_cast<int>(dat.size());
        const std::vector<uint8_t> ecc = QR::REEDSOLOMON::COMPUTE_REMAINDER(dat, rsDivisor);
        if (i < numShortBlocks)
            dat.push_back(0);
        dat.insert(dat.end(), ecc.cbegin(), ecc.cend());
        blocks.push_back(std::move(dat));
    }

    std::vector<std::uint8_t> result;
    for (size_t i = 0; i < blocks[0].size(); i++)
    {
        for (size_t j = 0; j < blocks.size(); j++)
        {
            if (i != static_cast<unsigned int>(shortBlockLen - blockEcc) ||
                j >= static_cast<unsigned int>(numShortBlocks))
                result.push_back(blocks[j][i]);
        }
    }
    assert(result.size() == static_cast<unsigned int>(rawCodeWords));
    return result;
}

inline void QR::QRCODE::DRAW_FUNCTIONS()
{
    for (int i = 0; i < size; i++)
    {
        SET_MODULE(6, i, i % 2 == 0);
        SET_MODULE(i, 6, i % 2 == 0);
    }

    POSITION_MARKER(3, 3);
    POSITION_MARKER(size - 4, 3);
    POSITION_MARKER(3, size - 4);

    const std::vector<int> align_patterns = ALIGMNET_PATTERN_GETTER();
    size_t numAlignmet = align_patterns.size();
    for (size_t i = 0; i < numAlignmet; i++)
    {
        for (size_t j = 0; j < numAlignmet; j++)
        {
            if (!((i == 0 && j == 0) ||
                (i == 0 && j == numAlignmet - 1) ||
                (i == numAlignmet - 1 && j == 0)))
                ALIGNMENT_MARKER(align_patterns[i], align_patterns[j]);
        }
    }

    DRAW_FORMAT_BITS(0);
    DRAW_VERSION();
}

inline void QR::QRCODE::DRAW_CODEWORDS(const std::vector<std::uint8_t>& data)
{
    if (data.size() != static_cast<unsigned int>(QRCODE::VERSION::GET_CAPACITY_BITS(version) / 8))
        throw std::domain_error("invalid argument");

    size_t i = 0;

    for (int right = size - 1; right >= 1; right -= 2)
    {
        if (right == 6)
            right = 5;
        for (int vert = 0; vert < size; vert++) {  // Vertical counter  
            for (int j = 0; j < 2; j++) {
                size_t x = static_cast<size_t>(right - j);  // Actual x coordinate
                bool upward = ((right + 1) & 2) == 0;
                size_t y = static_cast<size_t>(upward ? size - 1 - vert : vert);  // Actual y coordinate
                if (!isMasked.at(y).at(x) && i < data.size() * 8) {
                    Matrix.at(y).at(x) = BITBUFFER::BINARY_BITS(data.at(i >> 3), 7 - static_cast<int>(i & 7));
                    i++;
                }
            }
        }
    }
    assert(i == data.size() * 8);
}

inline void QR::QRCODE::POSITION_MARKER(int x, int y)
{
    for (int dy = -4; dy <= 4; dy++)
    {
        for (int dx = -4; dx <= 4; dx++)
        {
            int distance = std::max(std::abs(dx), std::abs(dy));
            int x_finder = x + dx, y_finder = y + dy;
            // Only set the module if it is within bounds of the QR code size.
            if (0 <= x_finder && 0 <= y_finder && x_finder < size && y_finder < size)
                QR::QRCODE::SET_MODULE(x_finder, y_finder, distance != 2 && distance != 4);
        }
    }
}

inline void QR::QRCODE::ALIGNMENT_MARKER(int x, int y)
{
    for (int dy = -2; dy <= 2; dy++)
    {
        for (int dx = -2; dx <= 2; dx++)
            QR::QRCODE::SET_MODULE(x + dx, y + dy, std::max(std::abs(dx), std::abs(dy)) != 1);
    }
}

inline int QR::QRCODE::PINALTY_COUNT_PATTERNS(const std::array<int, 7>& runHistory) const
{
    int n = runHistory[1];
    assert(n <= size * 3);
    bool core = n > 0 &&
        runHistory[2] == n &&
        runHistory[3] == n * 3 &&
        runHistory[4] == n &&
        runHistory[5] == n;
    return (core && runHistory.at(0) >= n * 4 && runHistory.at(6) >= n ? 1 : 0)
        + (core && runHistory.at(6) >= n * 4 && runHistory.at(0) >= n ? 1 : 0);
}

inline int QR::QRCODE::PINALTY_TERMINATE_COUNT(bool currentRunColor, int currentRunLength, std::array<int, 7>& runHistory) const
{
    if (currentRunColor) {  // Terminate dark run
        PENALTY_ADD_HISTORY(currentRunLength, runHistory);
        currentRunLength = 0;
    }
    currentRunLength += size;  // Add light border to final run
    PENALTY_ADD_HISTORY(currentRunLength, runHistory);
    return PINALTY_COUNT_PATTERNS(runHistory);
}

inline void QR::QRCODE::PENALTY_ADD_HISTORY(int currentRunLength, std::array<int, 7>& runHistory) const
{
    if (runHistory.at(0) == 0)
        currentRunLength += size;  // Add light border to initial run
    std::copy_backward(runHistory.cbegin(), runHistory.cend() - 1, runHistory.end());
    runHistory.at(0) = currentRunLength;
}

inline long QR::QRCODE::GET_PENALY_SCORE() const
{
    long result = 0;

    for (int y = 0; y < size; y++) {
        bool runColor = false;
        int runX = 0;
        std::array<int, 7> runHistory = {};
        for (int x = 0; x < size; x++) {
            if (MODULE(x, y) == runColor) {
                runX++;
                if (runX == 5)
                    result += PENALTY_N1;
                else if (runX > 5)
                    result++;
            }
            else {
                PENALTY_ADD_HISTORY(runX, runHistory);
                if (!runColor)
                    result += PINALTY_COUNT_PATTERNS(runHistory) * PENALTY_N3;
                runColor = MODULE(x, y);
                runX = 1;
            }
        }
        result += PINALTY_TERMINATE_COUNT(runColor, runX, runHistory) * PENALTY_N3;
    }
    // Adjacent modules in column having same color, and finder-like patterns
    for (int x = 0; x < size; x++) {
        bool runColor = false;
        int runY = 0;
        std::array<int, 7> runHistory = {};
        for (int y = 0; y < size; y++) {
            if (MODULE(x, y) == runColor) {
                runY++;
                if (runY == 5)
                    result += PENALTY_N1;
                else if (runY > 5)
                    result++;
            }
            else {
                PENALTY_ADD_HISTORY(runY, runHistory);
                if (!runColor)
                    result += PINALTY_COUNT_PATTERNS(runHistory) * PENALTY_N3;
                runColor = MODULE(x, y);
                runY = 1;
            }
        }
        result += PINALTY_TERMINATE_COUNT(runColor, runY, runHistory) * PENALTY_N3;
    }

    // 2*2 blocks of modules having same color
    for (int y = 0; y < size - 1; y++) {
        for (int x = 0; x < size - 1; x++) {
            bool  color = MODULE(x, y);
            if (color == MODULE(x + 1, y) &&
                color == MODULE(x, y + 1) &&
                color == MODULE(x + 1, y + 1))
                result += PENALTY_N2;
        }
    }

    // Balance of dark and light modules
    int dark = 0;
    for (const std::vector<bool>& row : Matrix) {
        for (bool color : row) {
            if (color)
                dark++;
        }
    }
    int total = size * size;  // Note that size is odd, so dark/total != 1/2
    // Compute the smallest integer k >= 0 such that (45-5k)% <= dark/total <= (55+5k)%
    int k = static_cast<int>((std::abs(dark * 20L - total * 10L) + total - 1) / total) - 1;
    assert(0 <= k && k <= 9);
    result += k * PENALTY_N4;
    assert(0 <= result && result <= 2568888L);  // Non-tight upper bound based on default values of PENALTY_N1, ..., N4
    return result;


}

inline const std::vector<std::vector<bool>> QR::QRCODE::MATRIX_GETTER() const
{
    return Matrix;
}

const int QR::QRCODE::PENALTY_N1 = 3;
const int QR::QRCODE::PENALTY_N2 = 3;
const int QR::QRCODE::PENALTY_N3 = 40;
const int QR::QRCODE::PENALTY_N4 = 10;

const int8_t QR::QRCODE::VERSION::ECC_CODEWORDS_PER_BLOCK[4][41] = {
    //0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
    {-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
    {-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
    {-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
    {-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
};

const int8_t QR::QRCODE::VERSION::NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
    //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
    {-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
    {-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
    {-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
    {-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};

QR::data_too_long::data_too_long(const std::string& message) :
    std::length_error(message) {}

#endif
