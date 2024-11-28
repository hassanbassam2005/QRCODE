#include "Image.hpp"


inline int IMAGE::COLOR::COSTUME_COLOR_RGB(int r, int g, int b)
{
	return 16 + (36 * (r / 51)) + (6 * (g/51)) + b/51; //devided each one by 51 to make sure the number stays between 0 and 225
}

inline void IMAGE::DEFAULT::PRINT_QR(const QR::QRCODE& qr)
{
	for (int y = -1; y < qr.SIZE_GETTER() + 1; y++) {
		for (int x = -1; x < qr.SIZE_GETTER() + 1; x++) {
			std::cout << (qr.GET_MODULE(x, y) ? "\033[48;5;0m  \033[0m" : "\033[48;5;15m  \033[0m");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

inline void IMAGE::DEFAULT::PRINT_QR(const QR::QRCODE& qr, int r, int g, int b)
{
	int colored = COLOR::COSTUME_COLOR_RGB(r, g, b);
	int uncolored = COLOR::COSTUME_COLOR_RGB(255 - r, 255 - g, 255- b);

	if ((r == 0 && g == 0 && b == 0) || (r == 255 && g == 255 && b == 255))
		std::swap(uncolored, colored);	
	if (r == g && b || b == r && g)
	{
		colored = 15;
		uncolored = 0;
	}

	for (int y = -1; y < qr.SIZE_GETTER() + 1; y++) {
		for (int x = -1; x < qr.SIZE_GETTER() + 1; x++) {
			if (qr.GET_MODULE(x, y))
			{
				std::cout << "\033[48;5;" << uncolored << "m  \033[0m";
			}
			else
			{
				std::cout << "\033[48;5;" << colored << "m  \033[0m";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

std::string IMAGE::SVG::SVG_STRING(const QR::QRCODE& qr,int border)
{
	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.SIZE_GETTER() + border * 2) << " " << (qr.SIZE_GETTER() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
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
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";
	return sb.str();
}

std::string IMAGE::SVG::SVG_STRING(const QR::QRCODE& qr, int border, int r, int g, int b)
{
	int colored = COLOR::COSTUME_COLOR_RGB(r, g, b);
	int uncolored = COLOR::COSTUME_COLOR_RGB(255 - r, 255 - g, 255 - b);

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
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\""<<uncolored<<"/>\n";
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
	sb << "\" fill=\"#000000\""<<colored<<"/ >\n";
	sb << "</svg>\n";
	return sb.str();

}
