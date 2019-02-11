#pragma once

int Decompression(FILE *PointerInput, FILE *PointerOutput)
{
	// Строка таблицы
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
  int iNumberOfCodes = 0;
  int iCurrentPrefixCode = -1;
  int iCurrentCode;
  int iOldCode;
  uchar cCurrentCharacter;
  int iCounter=0, iKbCounter=0;
  iNumberOfCodes=Dictionary(iPrefixCode, cCharacter);
  iCurrentCode=GetCodeFile(PointerInput);
  if(OutputStringCode(iPrefixCode, cCharacter, PointerOutput, iCurrentCode)<0)
  {
    Delete(iPrefixCode, cCharacter);
    return -1;
  }
  iOldCode=iCurrentCode;
  while((iCurrentCode=GetCodeFile(PointerInput))>=0) //// Счётчик
  {
	  iCounter++;
    if(iCounter>=((1024*8)/AmountOfBits))
    {
      iKbCounter++;
      iCounter-=((1024*8)/AmountOfBits);
      printf("%10d Kb decompressed...\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r", iKbCounter);
    }
    if(iCurrentCode==SizeOfTable)
    {
      iNumberOfCodes=Dictionary(iPrefixCode, cCharacter);
      iCurrentCode=GetCodeFile(PointerInput);
       
      iCounter++; // Счётчик
      if(iCounter>=((1024*8)/AmountOfBits))
      {
        iKbCounter++;
        iCounter-=((1024*8)/AmountOfBits);
        printf("%10d Kb decompressed...\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r", iKbCounter);
    	}
   		if(iCurrentCode<0)
    	  break;
      if(OutputStringCode(iPrefixCode, cCharacter, PointerOutput, iCurrentCode)<0)
      {
	      Delete(iPrefixCode, cCharacter);
	      return -1;
      }
      iOldCode=iCurrentCode;
    }
    else
    {
    	if(iCurrentCode<=iNumberOfCodes-1)
     	{
      	if(OutputStringCode(iPrefixCode, cCharacter, PointerOutput, iCurrentCode)<0)
      	{
         	Delete(iPrefixCode, cCharacter);
         	return -1;
      	}
        iCurrentPrefixCode=iOldCode;
        cCurrentCharacter=FirstCharacterCode(iPrefixCode, cCharacter, iCurrentCode);
        iPrefixCode[iNumberOfCodes]=iCurrentPrefixCode;
        cCharacter[iNumberOfCodes]=cCurrentCharacter;
        iNumberOfCodes++;
      	iOldCode=iCurrentCode;
      }
      else
      {
        iCurrentPrefixCode=iOldCode;
        cCurrentCharacter=FirstCharacterCode(iPrefixCode, cCharacter, iCurrentPrefixCode);
        iPrefixCode[iNumberOfCodes]=iCurrentPrefixCode;
        cCharacter[iNumberOfCodes]=cCurrentCharacter;
        iNumberOfCodes++;
        if(OutputStringCode(iPrefixCode, cCharacter, PointerOutput, iNumberOfCodes-1)<0)
      	{
          Delete(iPrefixCode, cCharacter);
          return -1;
      	}
        iOldCode=iCurrentCode;
      }
    }
  }
  Delete(iPrefixCode, cCharacter);
  return 0;
}

