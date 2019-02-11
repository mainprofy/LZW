#ifndef FUNCTIONS
#define FUNCTIONS

int Compression(FILE *PointerInput, FILE *PointerOutput);
int Decompression(FILE *PointerInput, FILE *PointerOutput);
int SearchInStringTable(int *iPrefixCode, uchar *cCharacter, int iNumberOfCodes, int iSearchingPrefixCode, uchar cSearchingCharacter);
int OutputCode(FILE *PointerOutput,int iCode);
int Dictionary(int *iaPrefixCode, uchar *caCharacter);
int GetCodeFile(FILE *PointerInput);
int OutputStringCode(int *iPrefixCode, uchar *cCharacter, FILE *PointerOutput, int iCurrentCode);
uchar FirstCharacterCode(int *iPrefixCode, uchar *cCharacter, int iCurrentCode);
void Delete(int *iPrefixCode, uchar *cCharacter);

#endif