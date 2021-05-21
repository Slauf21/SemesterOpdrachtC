#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __DEBUG

void compressBitwise(unsigned char *pArrayBits, unsigned char *inputPixels, int i, int imageSize);

void compress(unsigned char *inputPixels, int imageSize, FILE *fp, unsigned char *bmpHeader);

int main(int argc, char* argv[])
{
	FILE *fp = NULL;
	FILE *inputFilePointer = NULL;
	FILE *outputBMP = NULL;
	FILE *outputTXT = NULL;
	unsigned char R = 0;
	
	//Commandos maken:
	if (argc > 1)
	{	
		if (strcmp(argv[1], "--help") == 0)
		{
			printf("Input formaten:\n");
			printf("Compress: test -c -s inputtxtfile -i inputbmpfile -o outputbmpfile\n");
			printf("Decompress: test -d -i inputbmp -o outputtxtfile\n");
			return 0;
		}
		else
		{
			printf("Type test --help");
		}
		//We willen compressen
		if (strcmp(argv[1], "-c") == 0 && strcmp(argv[2], "-s") == 0 && strcmp(argv[4], "-i") == 0 && strcmp(argv[6], "-o") == 0)
		{
			R = 1;
			fp = fopen(argv[3], "r");
			fopen("test.bmp","wb");
			fopen("outputBMP.bmp","wb");
			if (fp == NULL)
			{
				printf("Cant open txt file");
				exit(EXIT_FAILURE);
			}	
			
			inputFilePointer = fopen(argv[5], "rb");
			if (inputFilePointer == NULL)
			{
				printf("Cant open bmp file");
				exit(EXIT_FAILURE);
			}
		}	
		
		//We willen Decompress
		else if (strcmp(argv[1], "-d") == 0 && strcmp(argv[2], "-i") == 0 && strcmp(argv[4], "-o") == 0)
		{
			R = 2;
			inputFilePointer = fopen(argv[3], "rb");
			if (inputFilePointer == NULL)
			{
				printf("Cant open bmp file");
				exit(EXIT_FAILURE);
			}
		}
	}

	else 
	{
		printf("%s\n", "Verkeerde input, type --help voor de uitleg.");
		exit(EXIT_FAILURE);
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	
	//Lezen van de image:
    #ifdef __DEBUG
        printf("DEBUG info: BMP transformer\n");
    #endif

  

    unsigned char bmpHeader[54]; // voorzie een array van 54-bytes voor de BMP Header
    fread(bmpHeader, sizeof(unsigned char), 54, inputFilePointer); // lees de 54-byte header
	

    //Informatie uit de header (wikipedia)
    // haal de hoogte en breedte uit de header
    int breedte = *(int*)&bmpHeader[18];
    int hoogte = *(int*)&bmpHeader[22];

    #ifdef __DEBUG
        printf("DEBUG info: breedte = %d\n", breedte);
        printf("DEBUG info: hoogte = %d\n", hoogte);
    #endif

    int imageSize = 3 * breedte * hoogte; //ieder pixel heeft 3 byte data: rood, groen en blauw (RGB)
    unsigned char* inputPixels = (unsigned char *) calloc(imageSize, sizeof(unsigned char)); // allocate een array voor alle pixels
	printf("%d", imageSize);

	
    fread(inputPixels, sizeof(unsigned char), imageSize, inputFilePointer); // Lees alle pixels (de rest van de file
    fclose(inputFilePointer);
	
	for(int i = 0; i < imageSize-2; i+=3)
	{
		printf("pixel %d: B= %d, G=%d, R=%d\n", i, inputPixels[i], inputPixels[i+1], inputPixels[i+2]);
	}
   
	printf("\n");
	
	//--------------------------------------------------------------------------------------------------------------------
	if (R == 1)//Compress
	{
		compress(inputPixels, imageSize, fp, bmpHeader);
		
		outputBMP = fopen(argv[7],"wb");
	
		fwrite(bmpHeader, sizeof(unsigned char), 54, outputBMP);
		fwrite(inputPixels, sizeof(unsigned char), imageSize, outputBMP);
		
		free(inputPixels);
		fclose(fp);
		fclose(outputBMP);
	}
	
	else if (R == 2)//Decompress
	{
		
		unsigned char teller = 0;
		unsigned char arrayBits[8] = {0};
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		int letterteller = 0;
		unsigned char Zin[768] = {0};
		
		for (int i = 0; i < imageSize-2; i+=3)//Naar elke pixel kijken
		{
			printf("%d\n",i);
			printf("%d\n",imageSize-2);
			b = inputPixels[i];//Waarde tussen 0 - 255
			g = inputPixels[i+1];
			r = inputPixels[i+2];
			
			arrayBits[i] = (b & 1);//Eerste bit pakken 01100111 --> 00000001 1
			teller++;
			if (teller == 7)//We hebben 8 bits dus een letter
			{
				for (int i = 0; i < 8; i++)
				{
					Zin[letterteller] += arrayBits[i] << i;//De eerste element van Zin wordt de eerste letter 8 bits.
				}							//10110110			
				for (int i = 0; i < 8; i++)
				{
					arrayBits[i] = 0;//Array leegmaken
				}
				if (Zin[letterteller] == 255)//Of de letter 1111111 is
				{
					printf("TEST\n");
					break;
				}
				letterteller += 1;//Volgende letter gaan
				teller = 0;
			}
			
			arrayBits[i+1] = (g & 1);//11011000 --> 00000000 10
			teller++;
			if (teller == 7)
			{
				for (int i = 0; i < 8; i++)
				{
					Zin[letterteller] += arrayBits[i] << i;
				}
				for (int i = 0; i < 8; i++)
				{
					arrayBits[i] = 0;
				}
				if (Zin[letterteller] == 255)
				{
					printf("TEST\n");
					break;
				}
				letterteller += 1;
				teller = 0;
			}
			arrayBits[i+2] = (r & 1);//11111111 --> 00000001 101
			teller++;
			if (teller == 7)
			{
				for (int i = 0; i < 8; i++)
				{
					Zin[letterteller] += arrayBits[i] << i;
				}
				for (int i = 0; i < 8; i++)
				{
					arrayBits[i] = 0;
				}
				if (Zin[letterteller] == 255)
				{
					printf("TEST\n");
					break;
				}
				letterteller += 1;
				teller = 0;
			}	
		printf("%d%d%d%d%d%d%d%d\n",arrayBits[i],arrayBits[i+1],arrayBits[i+2],arrayBits[i+3],arrayBits[i+4],arrayBits[i+5],arrayBits[i+6],arrayBits[i+7] );
		}
		
		outputTXT = fopen(argv[5], "w");
		
		fwrite(Zin, sizeof(unsigned char),letterteller,outputTXT);
		
		for (int i = 0; i < letterteller; i++)
		{	
			printf("%c\n", Zin[i]);
		}
		
		
	}
	
    return 0;
}

void compress(unsigned char *inputPixels, int imageSize, FILE *fp, unsigned char *bmpHeader)
{
	unsigned char c = 0;

	unsigned char bit7 = 0,bit6= 0,bit5= 0,bit4= 0,bit3= 0,bit2= 0,bit1= 0,bit0= 0;
	unsigned char arrayBits[8];

	int binair = 0;
	unsigned char x = 1;

	int i = 7;
	
	do
	{
		c = fgetc(fp);		
		bit7=(c & 0x80)>>7;
		arrayBits[7] = bit7;
		bit6=(c & 0x40)>>6;
		arrayBits[6] = bit6;
		bit5=(c & 0x20)>>5;
		arrayBits[5] = bit5;
		bit4=(c & 0x10)>>4;
		arrayBits[4] = bit4;
		bit3=(c & 0x08)>>3;
		arrayBits[3] = bit3;
		bit2=(c & 0x04)>>2;
		arrayBits[2] = bit2;
		bit1=(c & 0x02)>>1;
		arrayBits[1] = bit1;
		bit0=c & 0x01;
		arrayBits[0] = bit0;
		
		printf("%d%d%d%d%d%d%d%d\n", bit7,bit6,bit5,bit4,bit3,bit2,bit1,bit0);
		
		compressBitwise(arrayBits,inputPixels,i,imageSize);
		
		i += 8;
		
		binair = (int) c;
		while (c > 0)
		{
			if ((c % 2) == 0)
			{
				binair = binair << 0;
				c = c / 2;				
			}		
			else
			{
				while (binair & x)
				{
					binair = binair ^ x;
					x <<= 1;
				}				
				binair = binair ^ x;
				c = c / 2;	
			}
		}
	}
	while (!feof(fp));
	
	for(int i = 0; i < imageSize-2; i+=3)
	{
		printf("pixel %d: B= %d, G=%d, R=%d\n", i, inputPixels[i], inputPixels[i+1], inputPixels[i+2]);
	}
}

void compressBitwise(unsigned char *pArrayBits, unsigned char *inputPixels, int i, int imageSize)
{
	//De letter in de pixels zetten
	if (i < imageSize-2)
	{	
		inputPixels[i-7] = (inputPixels[i-7] & 0xFE) | pArrayBits[7];
		
		inputPixels[i-6] = (inputPixels[i-6] & 0xFE) | pArrayBits[6];

		inputPixels[i-5] = (inputPixels[i-5] & 0xFE) | pArrayBits[5];
		
		inputPixels[i-4] = (inputPixels[i-4] & 0xFE) | pArrayBits[4];
		
		inputPixels[i-3] = (inputPixels[i-3] & 0xFE) | pArrayBits[3];
		
		inputPixels[i-2] = (inputPixels[i-2] & 0xFE) | pArrayBits[2];
		
		inputPixels[i-1] = (inputPixels[i-1] & 0xFE) | pArrayBits[1];
		
		inputPixels[i] = (inputPixels[i] & 0xFE) | pArrayBits[0];			
	}
}