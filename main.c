#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __DEBUG

#define BMPINPUTFILE "test.bmp"

void compress(unsigned char *pArrayBits, unsigned char *inputPixels, int i, int imageSize);

int main(int argc, char* argv[])
{
	
	//--------------------------------------------------------------------------------------------------------------------
	
	//Lezen van de image:
    #ifdef __DEBUG
        printf("DEBUG info: BMP transformer\n");
    #endif

    FILE* inputFilePointer = fopen(BMPINPUTFILE, "rb"); //maak een file pointer naar de afbeelding
    if(inputFilePointer == NULL) //Test of het open van de file gelukt is!
    {
        printf("Something went wrong while trying to open %s\n", BMPINPUTFILE);
        exit(EXIT_FAILURE);
    }

    #ifdef __DEBUG
        printf("DEBUG info: Opening File OK: %s\n", BMPINPUTFILE);
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
	

	
    fread(inputPixels, sizeof(unsigned char), imageSize, inputFilePointer); // Lees alle pixels (de rest van de file
    fclose(inputFilePointer);
	void printPixels(int imageSize ,int inputPixels[]);
   
    fclose(inputFilePointer);
	
	printf("\n");
	
	//--------------------------------------------------------------------------------------------------------------------
	
	//Lezen van de secret message:
	unsigned char c = 0;
	unsigned char bit7 = 0,bit6= 0,bit5= 0,bit4= 0,bit3= 0,bit2= 0,bit1= 0,bit0= 0;
	unsigned char arrayBits[7];
	FILE *fp = NULL;
	
	//Commandos maken:
	if (argc > 1)
	{	
		if (strcmp(argv[1], "--help") == 0)
		{
			printf("%s\n", "Dit is de uitleg van het programma");
			return 0;
		}
		
		if (strcmp(argv[1], "-c") == 0 && strcmp(argv[2], "-s") == 0)
		{
			fp = fopen(argv[3], "r");
			
			if (fp == NULL)
			{
				printf("Cant open file");
				exit(EXIT_FAILURE);
			}	
		}		
	}
	
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
		
		compress(arrayBits,inputPixels,i,imageSize);
		
		i += 8;
	}
	while (!feof(fp));
	
	void printPixels(int imageSize ,int inputPixels[]);
	
	free(inputPixels);
	fclose(fp);
    return 0;
}

void compress(unsigned char *pArrayBits, unsigned char *inputPixels, int i, int imageSize)
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