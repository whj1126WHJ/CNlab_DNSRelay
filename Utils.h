//
// Created by 王乾凱 on 2021/5/13.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#endif //PROJECT_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef char byte;

short byteArrayToShort(byte* b);

short byteArrayToShort_offset(byte* b, int offset);

byte* shortToByteArray(short i);

int byteToInt (byte b);

int byteArrayToInt(byte* b);

int byteArrayToInt_offset(byte* bytes, int offset);

byte* intToByteArray(int i);

char* byteToHexString (byte b);

char* byteArrayToHexString (byte* bytes);

char* byteArrayToAscii(byte* bytes, int offset, int dataLen, int len);

char* extractDomain(byte* bytes, int offset, int stopByte, int len);

byte* domainToByteArray(char* domain);

byte* ipv4ToByteArray(char* ipv4);

int split(char *src,const char *separator,char **dest);

void cStrcat(char* stringBuffe, byte *dat, int offset);