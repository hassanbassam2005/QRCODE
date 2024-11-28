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
		colored = 0xF;
		uncolored = 0x00;
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
