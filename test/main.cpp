#include "QRCode.h"

#include "../../QRCODE/lib/Image/Image.cpp"

#include <iostream>
#include <string>
#include <fstream>


using namespace QR;

//testing the buffer
static void testing_buffer()
{
	int l = 0;

	BITBUFFER bit;

	bit.APPEND_BITS(72);
	std::cout << "Bits in buffer: ";
	for (bool bitval : bit) {
		std::cout << bitval;
	}
	std::cout << std::endl;

	bit.clear();

	std::cout << "Testing the 2nd eppend... \nenter length of binary :" << std::endl;
	bit.APPEND_BITS(65, 10);
	std::cout << "Bits in buffer: ";
	for (bool bitval : bit) {
		std::cout << bitval;
	}
	std::cout << std::endl;

	bit.clear();

	std::cout << "Testing the 3d appnd... ";
	const char* T = { "H2" };
	for(;*T != '\0'; T++)
		bit.APPEND_BITS(static_cast<std::uint32_t>(*T));

	for (bool bitval : bit)
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
	
	for (bool bit : encoder.DATA_GETTER()) {
		std::cout << bit;
	}
	std::cout << std::endl;

	std::cout << encoder.SIZE_GETTER() << std::endl;
	
}

static void testing_Mode_Numeric()
{
	QR::ENCODE encoder = QR::ENCODE::MODE::NUMERIC_TO_BINARY("513445");
	for (bool bit : encoder.DATA_GETTER()) {
		std::cout << bit;
	}
	std::cout << std::endl;
	
	std::cout << encoder.SIZE_GETTER() << std::endl;
}

static void testing_Mode_Byte()
{
	std::vector<std::uint8_t> input = { 0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E,
										0x79, 0x6F, 0x75, 0x74, 0x75, 0x62, 0x65, 0x2E, 0x63, 0x6F, 0x6D, 0x2F };
	QR::ENCODE encoder = QR::ENCODE::MODE::BYTE_TO_BINARY(input);

	for (bool bit : encoder.DATA_GETTER()) {
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

static std::string toSvgString(const QRCODE& qr, int border) {
	if (border < 0)
		throw std::domain_error("Border must be non-negative");
	if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.SIZE_GETTER())
		throw std::overflow_error("Border too large");

	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.SIZE_GETTER() + border * 2) << " " << (qr.SIZE_GETTER() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	sb << "\t<path d=\"";
	for (int y = 0; y < qr.SIZE_GETTER(); y++) {
		for (int x = 0; x < qr.SIZE_GETTER(); x++) {
			if (qr.GET_MODULE(x, y)) {
				if (x != 0 || y != 0)
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";
	return sb.str();
}

static std::string toPPMString(const QRCODE& qr, int border)
{
	int size = qr.SIZE_GETTER();
	int imageSize = size + 2 * border; // Include the border in the output size

	// Initialize the PPM header
	std::string ppm = "P3\n";
	ppm += std::to_string(imageSize) + " " + std::to_string(imageSize) + "\n";
	ppm += "255\n"; // Max color value

	// Loop through each pixel in the output image
	for (int y = -border; y < size + border; ++y) {
		for (int x = -border; x < size + border; ++x) {
			if (x < 0 || y < 0 || x >= size || y >= size) {
				// Border area: White
				ppm += "255 255 255 ";
			}
			else {
				// Inside QR code: Black or White
				if (qr.GET_MODULE(x, y)) {
					ppm += "0 0 0 "; // Black
				}
				else {
					ppm += "255 255 255 "; // White
				}
			}
		}
		ppm += "\n"; // New line for each row
	}

	return ppm;
}

static void testing_capacity()
{
	QRCODE::VERSION qr1;
	int res1 = qr1.GET_CAPACITY_BITS(40);
	std::cout <<"result1 :" << res1 << std::endl;
	int res2 = qr1.GET_CAPACITY_CODEWORDS(40,QRCODE::VERSION::ERROR::LOW);
	std::cout <<"result2 :" << res2 << std::endl;

}

static void printQR(const QRCODE& qr)
{
	int border = 2;

	for (int y = -border; y < qr.SIZE_GETTER() + border; y++) {
		for (int x = -border; x < qr.SIZE_GETTER() + border; x++) {
			std::cout << (qr.GET_MODULE(x, y) ? "\033[48;5;0m  \033[0m" : "\033[48;5;15m  \033[0m");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
	

int main()
{
	//testing_buffer();
	//testing_Mode_();
	//testing_Mode_Numeric();
	//testing_Mode_Alphanumeric_Numeric();
	//testing_Mode_Byte();
	//tetsing_Version();
	//testing_capacity();
	const char* text = "https://www.youtube.com/";
	const QRCODE::VERSION::ERROR errorlvl = QRCODE::VERSION::ERROR::HIGH;
	const QRCODE qr = QRCODE::ENCODE_TEXT(text, errorlvl);
	IMAGE::DEFAULT::PRINT_QR(qr,5,5,5);
	std::string svg = toSvgString(qr,4);
	std::string ppm = toPPMString(qr, 4);
	std::ofstream file("qrcode.svg");
	std::ofstream files("qrcode.ppm");
	file << svg;
	files << ppm;
	std::cout << svg;

}