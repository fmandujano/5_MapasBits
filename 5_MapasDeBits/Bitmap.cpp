#include "Bitmap.h"

Bitmap::Bitmap()
{
	data = NULL;
	sizex = sizey = 0;
}
Bitmap::Bitmap(int x, int y)
{
	data = (Pixel*)calloc(x*y, sizeof(Pixel));
	if (data == NULL)
	{
		printf("error\n");
	}

	sizex = x;
	sizey = y;
}

Bitmap::~Bitmap()
{
	free(data);
	data = NULL;
}

void Bitmap::SetPixel(int x, int y, Pixel valor)
{
	//seguro
	if (data==NULL || x >= sizex || y >= sizey) 
		return;

	data[y * sizex + x] = valor;
}

Bitmap::Pixel Bitmap::GetPixel(int x, int y)
{
	//seguro
	if (data == NULL || x >= sizex || y >= sizey)
	{
		Pixel p;
		p.R = 0; p.G = 0; p.B = 0;
		return p;
	}
	else
	{
		return data[y*sizex + x];
	}
}

void Bitmap::Load(const char *filename)
{
	delete data;
	data = NULL;
	std::fstream myfile;
	myfile.open(filename, std::ios::in | std::ios::out |std::ios::binary);
	if (!myfile)
	{
		std::cout << "Error, no se puede abrir la imagen " << filename;
		myfile.close();
		return;
	}
	printf("sizeof bitmap file header: %zi\n",sizeof(tagBITMAPFILEHEADER));
	printf("sizeof bitmap info header: %zi\n", sizeof(tagBITMAPINFOHEADER));
	
	BITMAPFILEHEADER bmfh;

	//leer exactamente 14 bytes. Se podria usar sizeof pero poniendo pragma pack
	myfile.read( (char*)&bmfh, 14);

	//imprimir bytes
	byte * p = (byte*)&bmfh;
	for (int i = 0; i < sizeof bmfh; i++)
	{
		printf("%02X\n", p[i]);
	}

	//imprimir info del archivo
	puts("File info:");
	printf("Type : %hX \nsize:%lX\nreserved1: %hX\nreserved2: %hX\noffbits: %lX\n\n", 
		bmfh.bfType,
		bmfh.bfSize,
		bmfh.bfReserved1,
		bmfh.bfReserved2,
		bmfh.bfOffBits	
	);

	BITMAPINFOHEADER bmih;
	//Se podria usar sizeof pero poniendo pragma pack
	myfile.read((char*)&bmih, 40);

	if (bmih.biSize != 40)
	{
		puts("ERROR. BITMAPINFOHEADER esta corrupta (size!=40)");
		return;
	}

	//leer tamano de la imagen
	sizex = bmih.biWidth;
	sizey = bmih.biHeight;
	printf("Imagen de %zi x %zi px.\n", bmih.biWidth, bmih.biHeight);
	printf("Imagen de %zi bpp.\n", bmih.biBitCount);

	DWORD imgsize = bmfh.bfSize - bmfh.bfOffBits;
	printf("Tamano de la imagen en bytes: %zi\n", imgsize);

	//leer datos
	byte * buff = new byte[imgsize];
	//TODO manejar error si no puede asignar memoria

	//brincar a los datos de imagen
	myfile.seekg(bmfh.bfOffBits, myfile.beg);

	myfile.read(buff, imgsize);

	//si el ancho de la imagen no es multiplo de 4, se debe insertar un padding y al leer hay que quitarlo
	LONG realWidth, paddedWidth;
	realWidth = paddedWidth = bmih.biWidth * (bmih.biBitCount / 8);  //o sea 3 
	//calcula el padding
	while (paddedWidth % 4 != 0)
	{
		paddedWidth++;
	}
	long padding = paddedWidth - realWidth;
	printf("real width: %zi, padding: %zi \n", realWidth, padding);

	//preparar el buffer real de datos
	data = new Pixel[sizex * sizey];
	byte * databytes = (byte*)data;

	//los datos en un bmp estan invertidos, entonces leer las filas de abajo hacia arriba
	//posicion inicial en el arreglo de pixeles, en bytes
	LONG posData = sizex * sizey * sizeof Pixel - realWidth;
	printf("posdata %zi\n", posData);

	//prueba, ponerle unos colores a data.
	data[0].R = data[5].G = data[10].B = 0x28;

	for (LONG i = 0; i < imgsize;)
	{
		//para imprimir los valores de pixel y demostrar que estan invertidos
		for (LONG j = 0; j < paddedWidth; j++)
		{
			//printf("%hhX ", buff[i+j]);
		}
		printf("\n");
		//////////////////////////////////////////////////////////////////////

		//los colores tambien estan invertidos en BGR
		for (LONG j = 0; j < realWidth; j+=3)
		{
			databytes[posData + j + 0] = buff[i + j + 2]; //B a R
			databytes[posData + j + 1] = buff[i + j + 1]; // G es G
			databytes[posData + j + 2] = buff[i + j + 0]; // R a B
		}
		posData -= realWidth;
		i += paddedWidth;
	}

	//imprimir los colores de data
	puts("");
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			printf("(%hhX,%hhX,%hhX) ", data[i*sizex + j].R, data[i*sizex + j].G, data[i*sizex + j].B);
		}
		printf("\n");
	}
}

void Bitmap::SaveToDisk(char* filename)
{
	std::ofstream myfile;
	myfile.open(filename, std::ios::binary);

	//Por implementar

	myfile.close();
}
