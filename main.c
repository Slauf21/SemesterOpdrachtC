#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
#define __DEBUG  
  
void compressBitwise(unsigned char *pArrayBits, unsigned char *inputPixels, int i, int imageSize);  
void compress(unsigned char *inputPixels, int imageSize, FILE *fp);  
int decompress(int imageSize, int letterteller, unsigned char *inputPixels, unsigned char* Zin); 
int tellerControle(int arrayTeller); 
int main(int argc, char* argv[])  
{  
	FILE *fp = NULL;  
	FILE *inputFilePointer = NULL;  
	FILE *compressed = NULL;  
	FILE *outputTXT = NULL;  
	unsigned char R = 0;  
	  
	//Commandos maken:  
	if (argc > 1)  
	{	  
		if (strcmp(argv[1], "--help") == 0)  
		{  
			printf("Input formaten:\n"); 
			printf("Compress: %s -c -s inputtxtfile -i inputbmpfile -o outputbmpfile\n", argv[0]); 
			printf("Decompress: %s -d -i inputbmp -o outputtxtfile\n", argv[0]); 
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
	  
    #ifdef __DEBUG  
        printf("DEBUG info: BMP transformer\n");  
    #endif  
 
  
    #ifdef __DEBUG  
        //printf("DEBUG info: Opening File OK: %s\n", inputFilePointer);  
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
	for(int i =0; i < imageSize-2; i+=3)  
	{  
		printf("pixel %d: B= %d, G=%d, R=%d\n", i, inputPixels[i], inputPixels[i+1], inputPixels[i+2]);  
	}  
     
    fclose(inputFilePointer);  
      
	if (R == 1)//Compress  
	{  
		compress(inputPixels, imageSize, fp); 
		 
		compressed = fopen(argv[7],"wb");  
	 
		fwrite(bmpHeader, sizeof(unsigned char), 54, compressed);  
		fwrite(inputPixels, sizeof(unsigned char), imageSize, compressed);  
		 
		free(inputPixels);  
		fclose(fp);  
		fclose(compressed);  
	 
		return 0;  
	}  
 
	else if (R == 2)//Decompress  
	{  
		unsigned char* Zin = (unsigned char *) calloc(imageSize, sizeof(unsigned char));  
		int letterteller = 0;  
		 
		letterteller= decompress(imageSize, letterteller, inputPixels, Zin); 
		 
		outputTXT = fopen(argv[5], "w");  
		 
		fwrite(Zin, sizeof(unsigned char),letterteller,outputTXT); 	  
		 
		printf("%s", Zin);  
		 
		fclose(outputTXT);  
		free(inputPixels);  
		free(Zin);  
		return 0;		  
	}  
}  
  
 
void compress(unsigned char *inputPixels, int imageSize, FILE *fp)  
{ 
	unsigned char c = 0;  
 
	unsigned char bit7 = 0,bit6= 0,bit5= 0,bit4= 0,bit3= 0,bit2= 0,bit1= 0,bit0= 0;  
	unsigned char arrayBits[8];  
 
 
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
 
int decompress(int imageSize, int letterteller, unsigned char *inputPixels, unsigned char* Zin) 
{ 
	unsigned char teller = 0;  
	unsigned char arrayBits[8] = {0};  
	unsigned char r = 0;  
	unsigned char g = 0;  
	unsigned char b = 0;  
	int arrayTeller = 0;  
 
	for (int i = 0; i < imageSize-2; i+=3)//Naar elke pixel kijken  
	{  
		  
		b = inputPixels[i];//Waarde tussen 0 - 255  
		g = inputPixels[i+1];  
		r = inputPixels[i+2];  
		  
		arrayBits[arrayTeller] = (b & 1);//Eerste bit pakken 01100111 --> 00000001 1  
 
		arrayTeller=tellerControle(arrayTeller);  
		  
		teller++;  
		if (teller == 8)//We hebben 8 bits dus een letter  
		{  
			for (int j = 0; j < 8; j++)  
			{  
				Zin[letterteller] += arrayBits[j] << (7 - j);//De eerste element van Zin wordt de eerste letter 8 bits.	 
			}		  
			  
			for (int j = 0; j < 8; j++)  
			{  
				arrayBits[j] = 0;//Array leegmaken  
			}  
			if (Zin[letterteller] == 255)//Of de letter 1111111 is  
			{  
				break;  
			}  
			letterteller ++;//Volgende letter gaan  
			teller = 0;  
		}  
		  
		arrayBits[arrayTeller] = (g & 1);//Eerste bit pakken 01100111 --> 00000001   
 
		arrayTeller=tellerControle(arrayTeller);
		  
		teller++;  
		if (teller == 8)  
		{  
			for (int j = 0; j < 8; j++)  
			{  
				Zin[letterteller] += arrayBits[j] << (7 - j);  
			}  
			for (int j = 0; j < 8; j++)  
			{  
				arrayBits[j] = 0;  
			}  
			if (Zin[letterteller] == 255)  
			{  
				break;  
			}  
			letterteller ++;  
			teller = 0;  
		}  
		arrayBits[arrayTeller] = (r & 1);//Eerste bit pakken 01100111 --> 00000001 1  
 
		arrayTeller=tellerControle(arrayTeller);
		  
		teller++;  
		if (teller == 8)  
		{  
			for (int j = 0; j < 8; j++)  
			{  
				Zin[letterteller] += arrayBits[j] << (7 - j);  
			}  
			for (int j = 0; j < 8; j++)  
			{  
				arrayBits[j] = 0;  
			}  
			if (Zin[letterteller] == 255)  
			{  
				break;  
			}  
			letterteller ++;  
			teller = 0;  
		}  
	} 
	return letterteller;
}
int tellerControle(int arrayTeller)
{
	if (arrayTeller == 7)  
	{  
		arrayTeller = 0;  
	}  
	else  
	{  
		arrayTeller++;  
	}  
	return arrayTeller;
}