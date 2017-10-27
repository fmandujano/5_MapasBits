// 5_MapasDeBits.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bitmap.h"

int main()
{
	Bitmap *mapa = new Bitmap();
	mapa->Load("creeper.bmp");

	system("PAUSE");
    return 0;
}

