#include "../lib/QRCode/QRCode.h"
#include "../../QRCODE/lib/Image/Image.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace QR;

int main()
{
	const char* text = "https://github.com/hassanbassam2005/QRCODE";
	const QRCODE::VERSION::ERROR errorlvl = QRCODE::VERSION::ERROR::HIGH;
	QRCODE qr = QRCODE::ENCODE_TEXT(text, errorlvl);
	IMAGE image{};
	image.PRINT_QR(qr);
	image.PNG_FILE(qr, 4, "qr.png");
	image.PNG_FILE(qr, 4, "anotheqr.png",123,34,12);
	std::string svg = image.SVG_STRING(qr);
	std::ofstream file("qr.svg");
	file << svg;
	file.close();
}

