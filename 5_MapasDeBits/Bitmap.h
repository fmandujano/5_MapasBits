#pragma once
#pragma pack(push, 1)
#include <stdlib.h>
#include <iostream>
#include <fstream>


typedef char byte;
typedef unsigned short  WORD; // 2bytes
typedef unsigned long  DWORD; //4bytes
typedef long LONG; 

class Bitmap
{
public:
	//File information header
	//provides general information about the file
	typedef struct tagBITMAPFILEHEADER 
	{
		WORD	bfType;
		DWORD   bfSize;
		WORD	bfReserved1;
		WORD	bfReserved2;
		DWORD   bfOffBits;
	} BITMAPFILEHEADER, *PBITMAPFILEHEADER;



	//Bitmap information header
	//provides information specific to the image data
	typedef struct tagBITMAPINFOHEADER {
		DWORD  biSize;
		LONG   biWidth;
		LONG   biHeight;
		WORD   biPlanes;
		WORD   biBitCount;
		DWORD  biCompression;
		DWORD  biSizeImage;
		LONG   biXPelsPerMeter;
		LONG   biYPelsPerMeter;
		DWORD  biClrUsed;
		DWORD  biClrImportant;
	} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

	//Colour palette
	typedef struct tagRGBQUAD 
	{
		byte	rgbBlue;
		byte	rgbGreen;
		byte	rgbRed;
		byte	rgbReserved;
	} RGBQUAD;

	struct Pixel
	{
		byte R;
		byte G;
		byte B;
	};

	Bitmap();
	Bitmap(int x, int y);
	~Bitmap();

	void SetPixel(int x, int y, Pixel valor);
	Pixel GetPixel(int x, int y);

	void SaveToDisk(char* filename);
	void Load(const char* filename);

	int SizeX()
	{
		return sizex;
	}//getter y setters
	int SizeY()
	{
		return sizey;
	}

private:
	Pixel * data;
	int sizex;
	int sizey;
};


