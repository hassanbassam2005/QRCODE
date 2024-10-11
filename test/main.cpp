#include "QRCode.h"


#include <iostream>
#include <string>

using namespace QR;

//testing the buffer
static void testing_buffer()
{
	std::vector<bool> buffer;
	int l = 0;

	BITBUFFER<std::vector<bool>> bit(buffer);

	bit.APPEND_BITS(72);
	std::cout << "Bits in buffer: ";
	for (bool bitval : buffer) {
		std::cout << bitval;
	}
	std::cout << std::endl;

	buffer.clear();

	std::cout << "Testing the 2nd eppend... \nenter length of binary :" << std::endl;
	bit.APPEND_BITS(65, 10);
	std::cout << "Bits in buffer: ";
	for (bool bitval : buffer) {
		std::cout << bitval;
	}
	std::cout << std::endl;

	buffer.clear();

	std::cout << "Testing the 3d appnd... ";
	const char* T = { "H2" };
	for(;*T != '\0'; T++)
		bit.APPEND_BITS(static_cast<std::uint32_t>(*T));

	for (bool bitval : buffer)
		std::cout << bitval;
	std::cout << std::endl;
}

static void testing_Mode_()
{
	// Testing the 'IS_ALPHANUMERIC' function from the ENCODE::MODE class.
	// Check if the string "test" is alphanumeric.
	bool truthy_A = ENCODE::MODE::IS_ALPHANUMERIC("T3");
	if (truthy_A == false) {
		std::cout << "false"; // Output 'false' if the string is not alphanumeric.
	}
	else if (truthy_A == true) {
		std::cout << "true"; // Output 'true' if the string is alphanumeric.
	}
	std::cout << std::endl;

	// Testing the 'IS_NUMERIC' function from the ENCODE::MODE class.
	// Check if the string "2" is numeric.
	bool truthy_N = ENCODE::MODE::IS_NUMERIC("2");
	if (truthy_N == false) {
		std::cout << "false"; // Output 'false' if the string is not numeric.
	}
	else if (truthy_N == true) {
		std::cout << "true"; // Output 'true' if the string is numeric.
	}
	std::cout << std::endl;
}

static void testing_Mode_Alphanumeric_Numeric()
{
	QR::ENCODE encoder = QR::ENCODE::MODE::ALPHANUMERIC_TO_BINARY("HASSAN");
	BITBUFFER<std::vector<std::uint8_t>> Enca = encoder.DATA_GETTER();
	const std::vector<std::uint8_t>& binaryData = Enca.BUFFER_GETTER();
	for (bool bit : binaryData) {
		std::cout << bit;
	}
	std::cout << std::endl;

	std::cout << encoder.SIZE_GETTER() << std::endl;
	std::cout << encoder.MODE_GETTER() << std::endl;
	
}

static void testing_Mode_Numeric()
{
	QR::ENCODE encoder = QR::ENCODE::MODE::NUMERIC_TO_BINARY("513445");
	BITBUFFER<std::vector<std::uint8_t>> Enca = encoder.DATA_GETTER();
	const std::vector<std::uint8_t>& binaryData = Enca.BUFFER_GETTER();
	for (bool bit : binaryData) {
		std::cout << bit;
	}
	std::cout << std::endl;
	
	std::cout << encoder.SIZE_GETTER() << std::endl;
}

static void testing_Mode_Byte()
{
	std::vector<std::uint8_t> input = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
	QR::ENCODE encoder = QR::ENCODE::MODE::BYTE_TO_BINARY(input);
	BITBUFFER<std::vector<std::uint8_t>> Enca = encoder.DATA_GETTER();
	const std::vector<std::uint8_t>& binaryData = Enca.BUFFER_GETTER();
	for (bool bit : binaryData) {
		std::cout << bit;
	}
	std::cout << std::endl;

	std::cout << encoder.SIZE_GETTER() << std::endl;
}

static void tetsing_Version()
{
	std::cout << "tetsing the multiply : ";
	int a = 3;
	int b = 5;
	int result = QR::REEDSOLOMON::GF_MULTIPLY(a, b);
	std::cout << result << std::endl;

	std::cout << "testing the divisor";
	int t = 16;
	std::vector<std::uint8_t>g = QR::REEDSOLOMON::COMPUTE_DIVISOR(t);
	for(const auto& co : g )
	{
		std::cout <<" " << static_cast<int>(co) << " ";
	}
	std::cout << std::endl;
}

int main()
{
	//testing_buffer();
	//testing_Mode_();
	//testing_Mode_Numeric();
	testing_Mode_Alphanumeric_Numeric();
	//testing_Mode_Byte();
	//tetsing_Version();
}