#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "Globals.h"
#include "Functions.h"
#include "Compression.h"
#include "Decompression.h"
int main()
{
	char EntranceFile[256],
       ExitFile[256];
  FILE *PointerInput,
       *PointerOutput;
  char Reading=0;
    //Чтение командной строки
  std::cout << "Dear user! To compress the file, press the letter 'c', to decompress the file, press the letter 'd':\n";
  std::cin >> &Reading;
  std::cout << "Enter name of input file:\n";
  std::cin >> EntranceFile;
  std::cout << "Enter name of output file:\n";
  std::cin >> ExitFile;
  PointerInput = fopen(EntranceFile,"rb");
  PointerOutput = fopen(ExitFile,"wb");
  if (!PointerInput)
    std::cerr << "Input file is in use by another program or does not exist\n";
  if (!PointerOutput)
    std::cerr << "Output file is in use by another program or does not exist\n";
  if (Reading!='c' && Reading!='d')
  {           
  	std::cerr << "Error: can't recognize this operation!..\n";
  	return -1;
  }
  if (Reading == 'c')
    if(Compression(PointerInput, PointerOutput)>=0)
    {
      std::cout << "Compression finished successfully!\n";
    }
    else
    {
      std::cerr << "Compression failed!\n";
    }

  if (Reading=='d')
    if(Decompression(PointerInput, PointerOutput)>=0)
    {
      std::cout << "DeCompression finished successfully!\n";
    }
    else
    {
      std::cerr << "DeCompression failed!\n";
    }
  fclose(PointerInput);
  fclose(PointerOutput);
  return 0;
}
// .::Is the code for string (==prefix+character) in string table, when Compression::.
int SearchInStringTable(int *iPrefixCode, uchar *cCharacter, int iNumberOfCodes, int iSearchingPrefixCode, uchar cSearchingCharacter)
{
  int i = 0;
  while(i<iNumberOfCodes)
  {
  	if ((iPrefixCode[i]==iSearchingPrefixCode) && (cCharacter[i]==cSearchingCharacter))
  	  return i;
    i++;
  }
  return -1;
}
// Вывод кода в выходной файл
int OutputCode(FILE *PointerOutput,int iCode)
{
 	static uchar cBuffer;
	static int iNumberOfBitsInBuffer;
 	int i=0;
 	int i2_in_power_NOBI=1;
 	if(iCode==-2)
 	{
  	cBuffer=(uchar)0;
    iNumberOfBitsInBuffer=0;
    return 0;
  }
  if(iCode==-1)
  {
  	if(iNumberOfBitsInBuffer!=0)
  	  fprintf(PointerOutput,"%c",cBuffer);
    return 0;
  }
  if(iCode>=0)
  {
    while(i<iNumberOfBitsInBuffer)
    {
    	i2_in_power_NOBI*=2;
    	i++;
    }
  	i=0;
    while(i<AmountOfBits)
    {
    	cBuffer+=i2_in_power_NOBI*(iCode%2);
     	iCode/=2;
     	i2_in_power_NOBI*=2;
     	iNumberOfBitsInBuffer++;
     	i++;
     	if (iNumberOfBitsInBuffer>=8)
     	{
      	fprintf(PointerOutput,"%c",cBuffer);
      	cBuffer=(uchar)0;
      	iNumberOfBitsInBuffer=0;
      	i2_in_power_NOBI=1;
      }
    }
    return 0;
  }
  return -1;
}
// Инициализация строки таблицы
int Dictionary(int *iaPrefixCode, uchar *caCharacter)
{
 	int i = 0;
 	while(i<256)
 	{
  	iaPrefixCode[i]=-1;
    caCharacter[i]=(uchar)i;
    i++;
 	}
 	return i;
}
// Получение первого символа из строки для кода
uchar FirstCharacterCode(int *iPrefixCode, uchar *cCharacter, int iCurrentCode)
{
  while(iPrefixCode[iCurrentCode]>=0)
  {
  	iCurrentCode=iPrefixCode[iCurrentCode];
 	}
 	return (cCharacter[iCurrentCode]);
}
// Вывод строки для кода в выходной файл
int OutputStringCode(int *iPrefixCode, uchar *cCharacter, FILE *PointerOutput,int iCurrentCode)
{
	if(iCurrentCode>=0)
 	{
  	int iCode = iCurrentCode;
  	int iLengthOfString = 0, i;
  	while(iPrefixCode[iCode]>=0)
  	{
    	iCode=iPrefixCode[iCode];
     	iLengthOfString++;
  	}
  	uchar *sOutputtingString = new uchar[iLengthOfString+1];
    if (sOutputtingString==NULL)
    {
      std::cout << "The required memory isn't allocated!";
      return -1;
    }
  	iCode=iCurrentCode;
  	i=iLengthOfString;
   	while(i>=0)
  	{
    	sOutputtingString[i]=cCharacter[iCode];
     	iCode=iPrefixCode[iCode];
     	i--;
    }
  	i=0;
  	while(i<=iLengthOfString)
  	{
    	fprintf(PointerOutput,"%c",sOutputtingString[i]);
     	i++;
  	}
  	delete [] sOutputtingString;
  	return 0;
  }
  else return -1;
}
// Получение следующего кода из файла
int GetCodeFile(FILE *PointerInput)
{
 	static uchar cBuffer;
 	static int iNumberOfBitsInBuffer = 0;
 	int i = 0;
 	int i2_in_power_i = 1 ;
 	int iCode = 0;
 	while(i<AmountOfBits)
 	{
  	if(iNumberOfBitsInBuffer==0)
  	{
    	if(fscanf(PointerInput, "%c", &cBuffer)!=1)
    	  return -1;
     	iNumberOfBitsInBuffer=8;
  	}
  	iCode+=(cBuffer%2)*i2_in_power_i;
  	cBuffer/=2;
  	iNumberOfBitsInBuffer--;
  	i++;
  	i2_in_power_i*=2;
  }
  return iCode;
}
void Delete(int *iPrefixCode, uchar *cCharacter)
{
  delete [] iPrefixCode;
  delete [] cCharacter;
}