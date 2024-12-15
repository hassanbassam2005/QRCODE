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
		int x, y;
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
		void PRINT_QR( QR::QRCODE& qr);


		/**
		* @brief Prints the QR code with any color according to r,g,b.
		* @param r The red component of the foreground color (0-255).
		* @param g The green component of the foreground color (0-255).
		* @param b The blue component of the foreground color (0-255).
		* @param qr The QR code object to be printed.
		*/
		void PRINT_QR(QR::QRCODE& qr, int r, int g, int b);

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

		/**
		* @brief Generates a PNG file representation of the QR code with specified scaling and color.
		*
		* @param qr The QR code object to generate the PNG from.
		* @param scale The scale factor for each module (pixel) in the QR code.
		* @param filename The path to the file where the PNG will be saved.
		* @param r The red component of the QR code color (0-255).
		* @param g The green component of the QR code color (0-255).
		* @param b The blue component of the QR code color (0-255).
		*/
		void PNG_FILE(const QR::QRCODE& qr, int scale, const char* filename, int r, int g, int b);

	};

}

int QR::IMAGE::BLEND_ANSI_COLOR(int r, int g, int b)
{
	return 16 + (36 * (r / 51)) + (6 * (g / 51)) + b / 51; //devided each one by 51 to make sure the number stays between 0 and 255
}

void QR::IMAGE::PRINT_QR(QR::QRCODE& qr)
{
	PRINT_QR(qr, 0, 0, 0);
}

inline void QR::IMAGE::PRINT_QR(QR::QRCODE& qr, int r, int g, int b)
{
	int colored = BLEND_ANSI_COLOR(r, g, b);
	int uncolored = BLEND_ANSI_COLOR(255 - r, 255 - g, 255 - b);

	std::vector<std::vector<unsigned char>> vec = qr.CONVERT(qr.MATRIX_GETTER());

	for (int y = -1, j = 0; y < qr.SIZE_GETTER() + 1; y++, j++) {
		for (int x = -1, i = 0; x < qr.SIZE_GETTER() + 1; x++, i++) {
			if (qr.GET_MODULE(x, y))
			{
				vec[i][j] = 255;
				 std::cout << "\033[48;5;" << static_cast<int>(vec[i][j])<< "m  \033[0m";
			}
			else
			{
				vec[i][j] = 0;
				std::cout << "\033[48;5;" << static_cast<int>(vec[i][j]) << "m  \033[0m";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

inline void QR::IMAGE::PRINT_QR(const QR::QRCODE& qr, int color)
{
	for (int y = -1; y < qr.SIZE_GETTER() + 1; y++) {
		for (int x = -1; x < qr.SIZE_GETTER() + 1; x++) {
			int colored = color;
			int uncolored = 255 - color;
			if (qr.GET_MODULE(x, y))
			{
				std::cout << "\033[48;5;" << colored << "m  \033[0m";
			}
			else
			{
				std::cout << "\033[48;5;" << uncolored << "m  \033[0m";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

std::string QR::IMAGE::SVG_STRING(const QR::QRCODE& qr, int r, int g, int b)
{
	int border = 4;
	int colored = BLEND_ANSI_COLOR(r, g, b);
	int uncolored = BLEND_ANSI_COLOR(255 - r, 255 - g, 255 - b);

	if ((r == 0 && g == 0 && b == 0) || (r == 255 && g == 255 && b == 255))
		std::swap(uncolored, colored);
	if (r == g && b || b == r && g)
	{
		colored = 15;
		uncolored = 0;
	}

	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.SIZE_GETTER() + border * 2) << " " << (qr.SIZE_GETTER() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"" << colored << "/>\n";
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
	sb << "\" fill=\"#000000\"" << uncolored << "/ >\n";
	sb << "</svg>\n";
	return sb.str();

}

void QR::IMAGE::PNG_FILE(const QR::QRCODE& qr, int scale, const char* filename) {
	int border = 4;
	int imageSize = (qr.SIZE_GETTER() + 2 * border) * scale;
	std::vector<unsigned char> imageData(imageSize * imageSize);

	for (int row = -1; row < qr.SIZE_GETTER() + 1; ++row) {
		for (int col = -1; col < qr.SIZE_GETTER() + 1; ++col) {
			int startX = (border + col) * scale;
			int startY = (border + row) * scale;
			if (qr.GET_MODULE(row, col)) {
				for (int y = startY; y < startY + scale; ++y) {
					for (int x = startX; x < startX + scale; ++x) {
						imageData[y * imageSize + x] = 255;
					}
				}
			}
			else
			{
				for (int y = startY; y < startY + scale; ++y) {
					for (int x = startX; x < startX + scale; ++x) {
						imageData[y * imageSize + x] = 0;
					}
				}
			}
		}
	}

	std::vector<unsigned char> png;
	unsigned error = lodepng::encode(png, imageData, imageSize, imageSize, LCT_GREY, 8);

	if (!error) {
		lodepng::save_file(png, filename);
	}
	else {
		std::cerr << "Error saving PNG: " << lodepng_error_text(error) << std::endl;
	}

}

void QR::IMAGE::PNG_FILE(const QR::QRCODE& qr, int scale, const char* filename, int r, int g, int b)
{
	int border = 1;
	int pixelSize = qr.SIZE_GETTER() + 2 * border;
	int imageSize = pixelSize * scale;

	std::vector<unsigned char> imageData(3 * imageSize * imageSize);

	for (int row = -border; row < qr.SIZE_GETTER() + border; ++row) {
		for (int col = -border; col < qr.SIZE_GETTER() + border; ++col) {
			int startX = (border + col) * scale;
			int startY = (border + row) * scale;
			if (qr.GET_MODULE(row, col)) {
				for (int y = startY; y < startY + scale; ++y) {
					for (int x = startX; x < startX + scale; ++x) {
						int index = 3 * (y * imageSize + x);
						imageData[index + 0] = r;
						imageData[index + 1] = g;
						imageData[index + 2] = b;
					}
				}
			}
			else {
				for (int y = startY; y < startY + scale; ++y) {
					for (int x = startX; x < startX + scale; ++x) {
						int index = 3 * (y * imageSize + x);
						imageData[index + 0] = 255 - r;
						imageData[index + 1] = 255 - g;
						imageData[index + 2] = 255 - b;
					}
				}
			}
		}
	}

	std::vector<unsigned char> png;
	unsigned error = lodepng::encode(png, imageData, imageSize, imageSize, LCT_RGB, 8);
	if (error) {
		std::cerr << "Error encoding PNG: " << lodepng_error_text(error) << std::endl;
		return;
	}

	lodepng::save_file(png, filename);
	std::cout << "PNG file saved as: " << filename << std::endl;
}

#endif
