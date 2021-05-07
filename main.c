#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __DEBUG

#define BMPINPUTFILE "test.bmp"
#define SECRETMSG "SecretMessage.txt"

int main(int argc, char* argv[])
{
	//--help commando maken:
	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			if (strcmp(argv[i], "--help") == 0)
			{
				printf("%s\n", "Dit is de uitleg van het programma");
				return 0;
			}
		}
	}	
	
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
	for(int i = 0; i < imageSize-2; i+=3)
	{
		printf("pixel %d: B= %d, G=%d, R=%d\n", i, inputPixels[i], inputPixels[i+1], inputPixels[i+2]);
	}
   
    fclose(inputFilePointer);
	
	printf("\n");
	
	//--------------------------------------------------------------------------------------------------------------------
	
	//Lezen van de secret message:
	FILE *fp = fopen(SECRETMSG, "r");
	unsigned char c = 0;
	unsigned char bit7 = 0,bit6= 0,bit5= 0,bit4= 0,bit3= 0,bit2= 0,bit1= 0,bit0= 0;
	if (fp == NULL)
	{
		printf("Cant open file");
		exit(EXIT_FAILURE);
	}	
	int i = 7;
	do
	{
		c = fgetc(fp);		
		bit7=(c & 0x80)>>7;
		bit6=(c & 0x40)>>6;
		bit5=(c & 0x20)>>5;
		bit4=(c & 0x10)>>4;
		bit3=(c & 0x08)>>3;
		bit2=(c & 0x04)>>2;
		bit1=(c & 0x02)>>1;
		bit0=c & 0x01;
		printf("%d%d%d%d%d%d%d%d\n", bit7,bit6,bit5,bit4,bit3,bit2,bit1,bit0);
		
		//De letter in de pixels zetten
		if (i < imageSize-2)
		{	
			inputPixels[i-7] = (inputPixels[i-7] & 0xFE) | bit7;
			
			inputPixels[i-6] = (inputPixels[i-6] & 0xFE) | bit6;
			
			inputPixels[i-5] = (inputPixels[i-5] & 0xFE) | bit5;
			
			inputPixels[i-4] = (inputPixels[i-4] & 0xFE) | bit4;
			
			inputPixels[i-3] = (inputPixels[i-3] & 0xFE) | bit3;
			
			inputPixels[i-2] = (inputPixels[i-2] & 0xFE) | bit2;
			
			inputPixels[i-1] = (inputPixels[i-1] & 0xFE) | bit1;
			
			inputPixels[i] = (inputPixels[i] & 0xFE) | bit0;			
		}
		
		i += 8;
	}
	while (!feof(fp));
	
	for(int i = 0; i < imageSize-2; i+=3)
	{
		printf("pixel %d: B= %d, G=%d, R=%d\n", i, inputPixels[i], inputPixels[i+1], inputPixels[i+2]);
	}
	
	free(inputPixels);
	fclose(fp);
    return 0;
}
//gcc -Wall -pedantic main.c -o test
