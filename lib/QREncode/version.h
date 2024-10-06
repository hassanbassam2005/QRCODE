#ifndef VERSION_H
#define VERSION_H

#include "../../lib/QREncode/QREncode.h"
#include <stdexcept>

namespace QR
{
	class VERSION : public ENCODE
	{
	public:
		const int MIN_VERSION = 1;
		const int MAX_VERSION = 40;

		enum class ERROR
		{
			LOW = 0,
			MEDUIM,
			QUARETILE,
			HIGH
		};
		int GETBITSERROR(ERROR err);

		
	};
}

int QR::VERSION::GETBITSERROR(ERROR err)
{
	switch (err)
	{
	case QR::VERSION::ERROR::LOW:
		return 1;
		break;
	case QR::VERSION::ERROR::MEDUIM:
		return 0;
		break;
	case QR::VERSION::ERROR::QUARETILE:
		return 2;
		break;
	case QR::VERSION::ERROR::HIGH:
		return 3;
		break;
	default:
		throw std::logic_error("Unreachable");
		break;
	}
}

#endif