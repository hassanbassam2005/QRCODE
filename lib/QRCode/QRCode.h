#ifndef QRCODE_H
#define QRCODE_H

#include"../BitBuffer/BitBuffer.h"
#include "../QREncode/QREncode.h"
#include"../ReedSolomon/ReedSolomon.h"
#include "../QRDesign/QRDesign.h"

namespace QR
{
	class QRCODE
	{
	private:
		int size;
		int mask;
		QR::ENCODE::VERSION::ERROR errorCorrectionLevel;
		
	public:
		QRCODE(int VERSION,
			   QR::ENCODE::VERSION::ERROR ECL,
			   std::vector<std::uint8_t>& DataCodeWords,
			   int MASK) {}
	};
}



#endif
