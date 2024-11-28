#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../../lib/QRCode/QRCode.h"

#include <cstdint>

namespace IMAGE
{
	struct COLOR
	{
		/**
		* @brief Converts RGB values to an 8-bit ANSI color index.
		*
		* This function maps the given RGB values (0-255 range for each component)
		* to a single 8-bit color code as used in ANSI terminal colors.
		* The mapping is performed according to the ANSI 256-color palette.
		*
		* @param r Red component (0-255).
		* @param g Green component (0-255).
		* @param b Blue component (0-255).
		* @return int The 8-bit ANSI color index corresponding to the RGB values.
		*/
		static int COSTUME_COLOR_RGB(int r, int g, int b);
	};

	struct DEFAULT
	{
		static void PRINT_QR(const QR::QRCODE& qr);
		static void PRINT_QR(const QR::QRCODE& qr, int r, int g,int b);
	};
}

#endif
