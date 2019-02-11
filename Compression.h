#pragma once

int Compression(FILE *PointerInput, FILE *PointerOutput)
{
	//строка таблицы
	int *iPrefixCode = new int[SizeOfTable];
	if (iPrefixCode==NULL)
	{
		std::cout << "The required memory isn't allocated!";
		return -1;
	}
  uchar *cCharacter = new uchar[SizeOfTable];
  if (cCharacter==NULL)
	{
		std::cout << "The required memory isn't allocated!";
		return -1;
	}
  int iCurrentPrefixCode = -1;
  uchar cCurrentCharacter;
  int iNumberOfCodes = 0;
  int iReturnCode;
  int iCounter=0, iKbCounter=0;
  iNumberOfCodes=Dictionary(iPrefixCode, cCharacter); // Инициализация базы словаря

  // Сжатие
  if(OutputCode(PointerOutput,-2)<0)
  {
  	Delete(iPrefixCode, cCharacter);
    return -1;
  }
  while(fscanf(PointerInput, "%c", &cCurrentCharacter)==1)
  {
  	// Счётчик
    iCounter++;
    if(iCounter>=1024)
    {
    	iKbCounter++;
      iCounter-=1024;
      printf("%10d Kb compressed...\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r", iKbCounter);
    }

    if ((iReturnCode=SearchInStringTable(iPrefixCode, cCharacter, iNumberOfCodes, iCurrentPrefixCode, cCurrentCharacter))>=0)
    {
     	iCurrentPrefixCode=iReturnCode;
    }
    else
    {
      iPrefixCode[iNumberOfCodes]=iCurrentPrefixCode;
      cCharacter[iNumberOfCodes]=cCurrentCharacter;
      iNumberOfCodes++;
      if(OutputCode(PointerOutput,iCurrentPrefixCode)<0)
     	{
        Delete(iPrefixCode, cCharacter);
        return -1;
      }
      iCurrentPrefixCode=(unsigned int)cCurrentCharacter;
    }

    if(iNumberOfCodes>=SizeOfTable)
    {
      if(OutputCode(PointerOutput,iCurrentPrefixCode)<0)
      {
        Delete(iPrefixCode, cCharacter);
        return -1;
      }
      iCurrentPrefixCode = -1;
      // Вывод кода для переполнения строки таблицы
      if(OutputCode(PointerOutput,SizeOfTable)<0)
      {
        Delete(iPrefixCode, cCharacter);
        return -1;
      }
      // Повторная инициализация базы словаря
      iNumberOfCodes=Dictionary(iPrefixCode, cCharacter);
    }
  }

  if(OutputCode(PointerOutput,iCurrentPrefixCode)<0)
  {
    Delete(iPrefixCode, cCharacter);
    return -1;
  }
  // Завершение сжатия
  if(OutputCode(PointerOutput,-1)<0)
  {
    Delete(iPrefixCode, cCharacter);
    return -1;
  }
  Delete(iPrefixCode, cCharacter);
  return 0;
}
