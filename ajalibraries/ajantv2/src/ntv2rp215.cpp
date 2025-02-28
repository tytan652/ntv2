/* SPDX-License-Identifier: MIT */
/**
	@file		ntv2rp215.cpp
	@brief		Implements the CNTV2RP215Decoder class. See SMPTE RP215 standard for details.
	@copyright	(C) 2006-2021 AJA Video Systems, Inc.
**/

#include "ntv2rp215.h"
#include "ntv2debug.h"
#include "ntv2utils.h"
#include "ntv2formatdescriptor.h"
#ifndef MSWindows
	using namespace std;
#endif

/////////////////////////////////////////////////////////////////////////////
// Line21Captioner definition
/////////////////////////////////////////////////////////////////////////////
#if defined (MSWindows)
#pragma warning(disable: 4800) 
#endif

/**	UNUSED
// Tables
// CRC Table
static const UWord CRC16_table[256] = {
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};


static void updateCRC16(UWord& crc, UByte data) {
	crc = CRC16_table[(crc >> 8) & 0xFF] ^ (crc << 8) ^ data;
}


	// Parity Table
	static const UWord Parity_table[256] = {
	0x200,0x101,0x102,0x203,0x104,0x205,0x206,0x107,
	0x108,0x209,0x20A,0x10B,0x20C,0x10D,0x10E,0x20F,
	0x110,0x211,0x212,0x113,0x214,0x115,0x116,0x217,
	0x218,0x119,0x11A,0x21B,0x11C,0x21D,0x21E,0x11F,
	0x120,0x221,0x222,0x123,0x224,0x125,0x126,0x227,
	0x228,0x129,0x12A,0x22B,0x12C,0x22D,0x22E,0x12F,
	0x230,0x131,0x132,0x233,0x134,0x235,0x236,0x137,
	0x138,0x239,0x23A,0x13B,0x23C,0x13D,0x13E,0x23F,
	0x140,0x241,0x242,0x143,0x244,0x145,0x146,0x247,
	0x248,0x149,0x14A,0x24B,0x14C,0x24D,0x24E,0x14F,
	0x250,0x151,0x152,0x253,0x154,0x255,0x256,0x157,
	0x158,0x259,0x25A,0x15B,0x25C,0x15D,0x15E,0x25F,
	0x260,0x161,0x162,0x263,0x164,0x265,0x266,0x167,
	0x168,0x269,0x26A,0x16B,0x26C,0x16D,0x16E,0x26F,
	0x170,0x271,0x272,0x173,0x274,0x175,0x176,0x277,
	0x278,0x179,0x17A,0x27B,0x17C,0x27D,0x27E,0x17F,
	0x180,0x281,0x282,0x183,0x284,0x185,0x186,0x287,
	0x288,0x189,0x18A,0x28B,0x18C,0x28D,0x28E,0x18F,
	0x290,0x191,0x192,0x293,0x194,0x295,0x296,0x197,
	0x198,0x299,0x29A,0x19B,0x29C,0x19D,0x19E,0x29F,
	0x2A0,0x1A1,0x1A2,0x2A3,0x1A4,0x2A5,0x2A6,0x1A7,
	0x1A8,0x2A9,0x2AA,0x1AB,0x2AC,0x1AD,0x1AE,0x2AF,
	0x1B0,0x2B1,0x2B2,0x1B3,0x2B4,0x1B5,0x1B6,0x2B7,
	0x2B8,0x1B9,0x1BA,0x2BB,0x1BC,0x2BD,0x2BE,0x1BF,
	0x2C0,0x1C1,0x1C2,0x2C3,0x1C4,0x2C5,0x2C6,0x1C7,
	0x1C8,0x2C9,0x2CA,0x1CB,0x2CC,0x1CD,0x1CE,0x2CF,
	0x1D0,0x2D1,0x2D2,0x1D3,0x2D4,0x1D5,0x1D6,0x2D7,
	0x2D8,0x1D9,0x1DA,0x2DB,0x1DC,0x2DD,0x2DE,0x1DF,
	0x1E0,0x2E1,0x2E2,0x1E3,0x2E4,0x1E5,0x1E6,0x2E7,
	0x2E8,0x1E9,0x1EA,0x2EB,0x1EC,0x2ED,0x2EE,0x1EF,
	0x2F0,0x1F1,0x1F2,0x2F3,0x1F4,0x2F5,0x2F6,0x1F7,
	0x1F8,0x2F9,0x2FA,0x1FB,0x2FC,0x1FD,0x1FE,0x2FF
	};
**/


UWord ancillaryPacketHeader[6] = 
{
	0x000,
	0x3FF,
	0x3FF,
	0x151,
	0x101,
	0x2D7
};

UWord raw215Data[] = 
{
		0x110,
		0x12C,
		0x107,
		0x224,
		0x256,
		0x116,
		0x200,
		0x200,
		0x200,
		0x200,
		0x218,
		0x115,
		0x108,
		0x205,
		0x200,
		0x200,
		0x108,
		0x200,
		0x200,
		0x24B,
		0x23F,
		0x102,
		0x217,
		0x104,
		0x108,
		0x140,
		0x176,
		0x287,
		0x104,
		0x146,
		0x146,
		0x146,
		0x146,
		0x107,
		0x138,
		0x107,
		0x101,
		0x101,
		0x200,
		0x200,
		0x200,
		0x200,
		0x2C3,
		0x104,
		0x214,
		0x104,
		0x120,
		0x143,
		0x2F6,
		0x27E,
		0x200,
		0x200,
		0x200,
		0x200,
		0x200,
		0x120,
		0x120,
		0x120,
		0x120,
		0x108,
		0x104,
		0x104,
		0x200,
		0x137,
		0x132,
		0x239,
		0x236,
		0x138,
		0x137,
		0x120,
		0x120,
		0x143,
		0x132,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x235,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x137,
		0x132,
		0x239,
		0x236,
		0x138,
		0x137,
		0x120,
		0x120,
		0x134,
		0x235,
		0x241,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x132,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x230,
		0x260,
		0x230,
		0x230,
		0x137,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x242,
		0x145,
		0x241,
		0x255,
		0x154,
		0x259,
		0x120,
		0x253,
		0x248,
		0x14F,
		0x250,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x146,
		0x14F,
		0x154,
		0x14F,
		0x24B,
		0x145,
		0x24D,
		0x120,
		0x132,
		0x22D,
		0x242,
		0x255,
		0x152,
		0x242,
		0x241,
		0x24E,
		0x24B,
		0x22D,
		0x255,
		0x253,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x120,
		0x16D,
		0x24E,
		0x13C
};



/////////////////////////////////////////////////////////////////////////////
// Constructor


CNTV2RP215Decoder::CNTV2RP215Decoder(ULWord* pFrameBufferBaseAddress,NTV2VideoFormat videoFormat,NTV2FrameBufferFormat fbFormat)
	: _frameBufferBasePointer(pFrameBufferBaseAddress),
	  _videoFormat(videoFormat),
	  _fbFormat(fbFormat),
      _lineNumber(-1),
      _pixelNumber(-1)
{
#if 0
	UWord crc=0;
	for ( int i=0; i<213; i++)
	{
		updateCRC16(crc,(UByte)raw215Data[i]);
		odprintf("0x%04X\n",crc);
		if ( crc == 0x6D4E )
			break;

	}
	crc = crc;
#endif
}



CNTV2RP215Decoder::~CNTV2RP215Decoder ()
{
}

bool CNTV2RP215Decoder::Locate()
{
	bool found = false;

	NTV2FormatDescriptor fd (_videoFormat,_fbFormat);
	UWord* rp215Linebuffer = new UWord[fd.numPixels*2];
	switch (_fbFormat )
	{
	case NTV2_FBF_10BIT_DPX:
		{
			ULWord* frameBuffer = _frameBufferBasePointer;
			for ( Word lineNumber=0; lineNumber<30 && found == false;lineNumber++)
			{
				::UnPack10BitDPXtoForRP215withEndianSwap(rp215Linebuffer,frameBuffer,fd.numPixels);
				for ( UWord pixelNumber=0;pixelNumber<(fd.numPixels-RP215_PAYLOADSIZE);pixelNumber++)
				{
					if ( rp215Linebuffer[0] == 0x000 && 
						 rp215Linebuffer[1] == 0x3ff &&
						 rp215Linebuffer[2] == 0x3ff &&
						 rp215Linebuffer[3] == 0x151 &&
						 rp215Linebuffer[4] == 0x101
						)

					{
						found = true;
						_lineNumber =  lineNumber;
						_pixelNumber = pixelNumber;
					}
				}
				frameBuffer += fd.linePitch;
			}
		}
		break;
    case NTV2_FBF_10BIT_DPX_LE:
		break;
	case NTV2_FBF_10BIT_YCBCR:
		{
			const ULWord* frameBuffer = _frameBufferBasePointer;
			for (int lineNumber = 0; lineNumber < 20 && found == false ; lineNumber ++)
			{
				::UnpackLine_10BitYUVto16BitYUV(frameBuffer, rp215Linebuffer, fd.numPixels);
				if ( rp215Linebuffer[1] == 0x000 && 
					rp215Linebuffer[3] == 0x3ff &&
					rp215Linebuffer[5] == 0x3ff &&
					rp215Linebuffer[7] == 0x151 &&
					rp215Linebuffer[9] == 0x101    )
				{
					UWord* pBuffer = &rp215Linebuffer[13];
					for ( int i=0; i < RP215_PAYLOADSIZE; i++ )
					{
						_rp215RawBuffer[i] = (UByte)(*pBuffer);
						pBuffer += 2;
					}
					found = true;
					_lineNumber =  lineNumber;
					_pixelNumber = 0;
					odprintf("found l(%d) p(%d)\n",_lineNumber,_pixelNumber);

				}
				frameBuffer += fd.linePitch;

			}
		}
		break;
	default:
		break;
	}
	
	delete [] rp215Linebuffer;

	return found;
}


bool CNTV2RP215Decoder::Extract()
{
	return false;
}


#if 0
#include "QTMovie.h"

bool TestQTMovieDecode(char* fileName)
{
	bool status = false;
	CQTMovieDec qtMovieDecoder;
	QTCodecInfo codec;
	codec.quality = codecLosslessQuality;
	memset(codec.CodecName,0,sizeof(codec.CodecName));
	memset(codec.strFourCC,0,sizeof(codec.strFourCC));
	qtMovieDecoder.OpenMovie(fileName,codec,1,1);
	qtMovieDecoder.InitializeGraphics(false, false, false, false);
	codec = qtMovieDecoder.QTGetCodecInfo();
	ULWord frameCount = qtMovieDecoder.GetVideoDuration();
	ULWord rawSize = qtMovieDecoder.GetRawFrameSize();
	byte* pFrameBuffer = new byte[rawSize];

	for ( int i=0; i<frameCount  ; i++ )
	{
		qtMovieDecoder.ReadVideoFrame(i,pFrameBuffer);
		CNTV2RP215Decoder rp215Decoder((ULWord*)pFrameBuffer,NTV2_FORMAT_1080i_5994,NTV2_FBF_10BIT_YCBCR);
		if ( rp215Decoder.Locate())
			status = true;
	}
	delete [] pFrameBuffer;

	return true;
}



bool FindRP215Data(ULWord* frameBuffer, UWord numPixels)
{
	bool status = false;
	UWord* yCbCrLine = new UWord[numPixels*2];

	for (int lineCount = 0; lineCount < 10 && status == false ; lineCount ++)
	{
		::UnpackLine_10BitYUVto16BitYUV(frameBuffer, yCbCrLine, 1920);
		if ( yCbCrLine[1] == 0x000 && 
			yCbCrLine[3] == 0x3ff &&
			yCbCrLine[5] == 0x3ff &&
			yCbCrLine[7] == 0x151 &&
			yCbCrLine[9] == 0x101    )
		{
			for ( int i=0; i < 230; i++ )
			{
				odprintf("0x%03X,\n",yCbCrLine[(i*2)+1]);
			}
			status =  true;
		}
		frameBuffer += HD_YCBCRLINEPITCH_1080;

	}
	delete [] yCbCrLine;
	return status;
}
#endif	//	if 0

#ifdef MSWindows
#pragma warning(default: 4800)
#endif
