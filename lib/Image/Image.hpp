#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../../lib/QRCode/QRCode.h"
#include "../pngLoader/lodepng/lodepng.cpp"

#include <cstdint>
#include <fstream>

namespace QR
{
	struct IMAGE
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
		int BLEND_ANSI_COLOR(int r, int g, int b);

		/**
		* @brief Prints the QR code with default colors (black and white).
		* @param qr The QR code object to be printed.
		*/
		void PRINT_QR(const QR::QRCODE& qr);


		/**
		* @brief Prints the QR code with any color according to r,g,b.
		* @param r The red component of the foreground color (0-255).
		* @param g The green component of the foreground color (0-255).
		* @param b The blue component of the foreground color (0-255).
		* @param qr The QR code object to be printed.
		*/
		void PRINT_QR(const QR::QRCODE& qr, int r, int g, int b);

		/**
		* @brief Prints the QR code with any color (from 0 till 255).
		* @param qr The QR code object to be printed.
		*/
		void PRINT_QR(const QR::QRCODE& qr, int color);

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
		std::string SVG_STRING(const QR::QRCODE& qr, int r, int g, int b);


		/**
		* @brief Generates a PNG file representation of the QR code.
		*
		* @param qrMatrix The QR code object to generate the PNG from.
		* @param scale The scale factor for each module (pixel) in the QR code.
		* @param filename The path to the file where the PNG will be saved.
		*/
		void PNG_FILE(const QR::QRCODE& qr, int scale, const char* filename);



	};

}

#endif
