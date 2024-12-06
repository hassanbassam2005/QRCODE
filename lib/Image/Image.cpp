#include "Image.hpp"


using namespace QR;

int QR::IMAGE::BLEND_ANSI_COLOR(int r, int g, int b)
{
	return 16 + (36 * (r / 51)) + (6 * (g / 51)) + b / 51; //devided each one by 51 to make sure the number stays between 0 and 255
}

void QR::IMAGE::PRINT_QR(const QR::QRCODE& qr)
{
	PRINT_QR(qr, 0, 0, 0);
}

inline void IMAGE::PRINT_QR(const QR::QRCODE& qr,int r,int g,int b)
{
	int colored = BLEND_ANSI_COLOR(r, g, b);
	int uncolored = BLEND_ANSI_COLOR(255 - r, 255 - g, 255 - b);

	for (int y = -1,j = 0; y < qr.SIZE_GETTER() + 1; y++,j++) {
		for (int x = -1, i = 0; x < qr.SIZE_GETTER() + 1; x++, i++) {
			if (qr.GET_MODULE(x,y))
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

inline void IMAGE::PRINT_QR(const QR::QRCODE& qr,int color)
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

std::string IMAGE::SVG_STRING(const QR::QRCODE& qr,int r,int g,int b)
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
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\""<<colored<<"/>\n";
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
	sb << "\" fill=\"#000000\""<<uncolored<<"/ >\n";
	sb << "</svg>\n";
	return sb.str();

}

void IMAGE::PNG_FILE(const QR::QRCODE& qr,int scale, const char* filename) {
	int border = 4; // Default border size in modules
	int pixelSize = qr.SIZE_GETTER() + 2 * border;
	int imageSize = pixelSize * scale;

	// Initialize image data (white background)
	std::vector<unsigned char> imageData(imageSize * imageSize);

	// Draw the QR code matrix with scaling and borders
	for (int row = -1; row < qr.SIZE_GETTER() + 1; ++row) {
		for (int col = -1; col < qr.SIZE_GETTER() + 1; ++col) {
			int startX = (border + col) * scale;
			int startY = (border + row) * scale;
			if (qr.GET_MODULE(row, col)) {
				for (int y = startY; y < startY + scale; ++y) {
					for (int x = startX; x < startX + scale; ++x) {
						imageData[y * imageSize + x] = 0;
					}
				}
			}
			else
			{
				for (int y = startY; y < startY + scale; ++y) {
					for (int x = startX; x < startX + scale; ++x) {
						imageData[y * imageSize + x] = 255;
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

