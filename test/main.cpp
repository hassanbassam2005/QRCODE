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
	IMAGE image;
	image.PRINT_QR(qr);
	image.PNG_FILE(qr, 4, "qr.png");
	image.PNG_FILE(qr, 4, "anotheqr.png",123,34,12);
	std::vector<std::vector<unsigned char>> vec = qr.CONVERT(qr.MATRIX_GETTER());
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < vec.size(); j++)
		{
			std::cout << static_cast<int>(vec[i][j]) << " ";
		}
		std::cout << std::endl;
	}
}

