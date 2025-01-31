#include "../lib/QRCode/QRCode.h"
#include "../../QRCODE/lib/Image/Image.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace QR;

int main()
{
	const char* link = "https://support.broadcom.com/group/ecx/productdownloads?subfamily=VMware+Fusion";
	const QRCODE::VERSION::ERROR errorlvl = QRCODE::VERSION::ERROR::MEDIUM;
	QRCODE qr = QRCODE::ENCODE_TEXT(link, errorlvl);
	IMAGE image{};
	image.PRINT_QR(qr);
	image.PNG_FILE(qr, 6, "qr.png");
	image.PNG_FILE(qr, 4, "anotheqr.png",128,0,128);
}

