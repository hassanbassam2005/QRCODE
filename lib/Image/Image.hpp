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
		/**
		* @brief Prints the QR code with default colors (black and white).
		* @param qr The QR code object to be printed.
		*/
		static void PRINT_QR(const QR::QRCODE& qr);

		/**
		 * @brief Prints the QR code with a custom color specified by the RGB values.
		 * @param qr The QR code object to be printed.
		 * @param r The red component of the custom color (0-255).
		 * @param g The green component of the custom color (0-255).
		 * @param b The blue component of the custom color (0-255).
		 */
		static void PRINT_QR(const QR::QRCODE& qr, int r, int g, int b);

	};
}

#endif
