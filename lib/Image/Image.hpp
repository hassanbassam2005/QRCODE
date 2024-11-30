#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../../lib/QRCode/QRCode.h"

#include <cstdint>
#include <fstream>

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
		static int BLEND_ANSI_COLOR(int r, int g, int b);
		
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

		static void PRINT_QR(const QR::QRCODE& qr, int color);

	};

	struct SVG
	{
		/**
		* @brief Generates an SVG string representation of the QR code with a default black and white color scheme.
		*
		* @param qr The QR code object to generate the SVG from.
		* @param border The size of the border around the QR code in the SVG.
		* @return A string containing the SVG representation of the QR code.
		*/
		static std::string SVG_STRING(const QR::QRCODE& qr, int border);

		/**
		 * @brief Generates an SVG string representation of the QR code with custom foreground and background colors.
		 *
		 * @param qr The QR code object to generate the SVG from.
		 * @param border The size of the border around the QR code in the SVG.
		 * @param r The red component of the foreground color (0-255).
		 * @param g The green component of the foreground color (0-255).
		 * @param b The blue component of the foreground color (0-255).
		 * @return A string containing the SVG representation of the QR code with the specified colors.
		 */
		static std::string SVG_STRING(const QR::QRCODE& qr, int border, int r, int g, int b);
	};



}

#endif
