#include "../lib/QRCode/QRCode.h"
#include "../../QRCODE/lib/Image/Image.cpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace QR;

int main()
{
	const char* text = "https://github.com/hassanbassam2005/QRCODE";
	const QRCODE::VERSION::ERROR errorlvl = QRCODE::VERSION::ERROR::HIGH;
	const QRCODE qr = QRCODE::ENCODE_TEXT(text, errorlvl);
	IMAGE image;
	image.PRINT_QR(qr);
	image.PNG_FILE(qr, 4, "qr.png");
}